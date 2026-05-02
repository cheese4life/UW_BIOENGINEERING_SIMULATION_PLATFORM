#!/usr/bin/env python3
"""
DOF-5 Up/Down Motion Test
===========================
Simulates a DOF-5 focus stage optic assembly moving up and down,
generates realistic trace data, plots to PNG, and displays on
the Pi's external screen via framebuffer.

The DOF-5 has ~5mm travel range, so we simulate repeated 2mm
up/down cycles with realistic motion profiles (s-curve acceleration,
position error, motor current).
"""

import os
import sys
import math
import csv
import subprocess
import numpy as np

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

OUTPUT_DIR = os.path.expanduser("~/dof_dashboard/output")
os.makedirs(OUTPUT_DIR, exist_ok=True)


# ---------------------------------------------------------------------------
# DOF-5 Motion Simulator
# ---------------------------------------------------------------------------

def s_curve_profile(t, t_start, t_end, pos_start, pos_end,
                    accel_frac=0.25, decel_frac=0.25):
    """Generate S-curve motion profile position at time t."""
    duration = t_end - t_start
    if duration <= 0:
        return pos_start

    tn = (t - t_start) / duration
    tn = max(0.0, min(1.0, tn))
    distance = pos_end - pos_start

    if tn <= accel_frac:
        phase = tn / accel_frac
        s = accel_frac * (phase - math.sin(math.pi * phase) / math.pi)
    elif tn <= (1.0 - decel_frac):
        s = accel_frac * 1.0 + (tn - accel_frac)
    else:
        phase = (tn - (1.0 - decel_frac)) / decel_frac
        s = (1.0 - decel_frac) + decel_frac * (phase - math.sin(math.pi * phase) / math.pi)

    s_total = 1.0
    frac = s / s_total
    return pos_start + distance * frac


def generate_dof5_updown_trace(n_cycles=3, travel_mm=2.0, move_time_s=0.2,
                                dwell_time_s=0.05, sample_period_s=0.000512):
    """
    Generate trace data for DOF-5 optic assembly moving up and down.
    """
    cycle_time = 2 * (move_time_s + dwell_time_s)
    total_time = n_cycles * cycle_time + dwell_time_s
    n_samples = int(total_time / sample_period_s) + 1
    rng = np.random.default_rng(42)

    time_arr = np.zeros(n_samples)
    cmd_pos = np.zeros(n_samples)
    act_pos = np.zeros(n_samples)
    pos_err = np.zeros(n_samples)
    cmd_vel = np.zeros(n_samples)
    motor_cur = np.zeros(n_samples)

    for i in range(n_samples):
        t = i * sample_period_s
        time_arr[i] = t
        cycle_pos = t % cycle_time

        if cycle_pos < move_time_s:
            cmd_pos[i] = s_curve_profile(cycle_pos, 0, move_time_s, 0, travel_mm)
        elif cycle_pos < move_time_s + dwell_time_s:
            cmd_pos[i] = travel_mm
        elif cycle_pos < 2 * move_time_s + dwell_time_s:
            cmd_pos[i] = s_curve_profile(cycle_pos, move_time_s + dwell_time_s,
                                          2 * move_time_s + dwell_time_s,
                                          travel_mm, 0)
        else:
            cmd_pos[i] = 0

    cmd_vel[0] = 0
    for i in range(1, n_samples):
        cmd_vel[i] = (cmd_pos[i] - cmd_pos[i-1]) / sample_period_s

    for i in range(n_samples):
        tracking_lag = cmd_vel[i] * 0.0002
        noise = rng.normal(0, 0.0001)
        act_pos[i] = cmd_pos[i] - tracking_lag + noise

    pos_err = cmd_pos - act_pos

    motor_cur[0] = 5.0
    for i in range(1, n_samples):
        accel = (cmd_vel[i] - cmd_vel[i-1]) / sample_period_s
        motor_cur[i] = 5.0 + accel * 0.005 + rng.normal(0, 2.0)

    return (time_arr, cmd_pos, act_pos, pos_err, cmd_vel, motor_cur)


# ---------------------------------------------------------------------------
# Save trace to CSV (MotionSynergy format)
# ---------------------------------------------------------------------------

def save_trace_csv(filepath, time_arr, cmd_pos, act_pos, pos_err, cmd_vel, motor_cur):
    with open(filepath, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([
            "Period (s)",
            "CommandedPosition (mm)",
            "ActualPosition (mm)",
            "PositionError (mm)",
            "CommandedVelocity (mm/s)",
            "PhaseAActualCurrent (mA)"
        ])
        for i in range(len(time_arr)):
            writer.writerow([
                f"{time_arr[i]:.6f}",
                f"{cmd_pos[i]:.6f}",
                f"{act_pos[i]:.6f}",
                f"{pos_err[i]:.6f}",
                f"{cmd_vel[i]:.6f}",
                f"{motor_cur[i]:.3f}"
            ])


# ---------------------------------------------------------------------------
# Plot for 800x480 screen
# ---------------------------------------------------------------------------

def plot_dof5_trace(time_arr, cmd_pos, act_pos, pos_err, cmd_vel, motor_cur,
                    output_path=None):
    """Create a 4-panel plot optimized for 800x480 display."""
    fig, axes = plt.subplots(4, 1, figsize=(8, 4.8), sharex=True,
                              gridspec_kw={"hspace": 0.35})

    fig.patch.set_facecolor("#0d1117")
    for ax in axes:
        ax.set_facecolor("#161b22")
        ax.tick_params(colors="#c9d1d9", labelsize=7)
        for spine in ax.spines.values():
            spine.set_color("#30363d")
        ax.grid(True, alpha=0.15, color="#c9d1d9")

    t_ms = time_arr * 1000

    # Panel 1: Position (commanded vs actual)
    axes[0].plot(t_ms, cmd_pos, color="#58a6ff", linewidth=0.8, label="Commanded")
    axes[0].plot(t_ms, act_pos, color="#f97583", linewidth=0.5, alpha=0.7, label="Actual")
    axes[0].set_ylabel("Position\n(mm)", fontsize=7, color="#c9d1d9")
    axes[0].legend(fontsize=6, loc="upper right", facecolor="#161b22",
                   edgecolor="#30363d", labelcolor="#c9d1d9")
    axes[0].set_title("DOF-5 Optic Assembly \u2014 Up/Down Motion Test",
                       fontsize=10, color="#58a6ff", fontweight="bold", pad=8)

    # Panel 2: Position Error
    pos_err_um = pos_err * 1000
    axes[1].plot(t_ms, pos_err_um, color="#f0883e", linewidth=0.6)
    axes[1].set_ylabel("Pos Error\n(\u00b5m)", fontsize=7, color="#c9d1d9")
    rms_err = np.sqrt(np.mean(pos_err_um**2))
    peak_err = np.max(np.abs(pos_err_um))
    axes[1].text(0.01, 0.92, f"RMS={rms_err:.3f}\u00b5m  Peak={peak_err:.3f}\u00b5m",
                 transform=axes[1].transAxes, fontsize=6, color="#7ee787",
                 verticalalignment="top",
                 bbox=dict(boxstyle="round,pad=0.2", facecolor="#0d1117", alpha=0.8))

    # Panel 3: Commanded Velocity
    axes[2].plot(t_ms, cmd_vel, color="#d2a8ff", linewidth=0.6)
    axes[2].set_ylabel("Velocity\n(mm/s)", fontsize=7, color="#c9d1d9")

    # Panel 4: Motor Current
    axes[3].plot(t_ms, motor_cur, color="#79c0ff", linewidth=0.4, alpha=0.8)
    axes[3].set_ylabel("Current\n(mA)", fontsize=7, color="#c9d1d9")
    axes[3].set_xlabel("Time (ms)", fontsize=8, color="#c9d1d9")

    if output_path is None:
        output_path = os.path.join(OUTPUT_DIR, "dof5_updown_test.png")

    fig.savefig(output_path, dpi=100, facecolor=fig.get_facecolor(),
                bbox_inches="tight", pad_inches=0.15)
    plt.close(fig)
    print(f"  Plot saved: {output_path}")
    return output_path


# ---------------------------------------------------------------------------
# Display on framebuffer
# ---------------------------------------------------------------------------

def display_on_screen(png_path):
    """Display PNG on the Pi's external screen via framebuffer."""
    print(f"  Displaying on screen...")

    # Try fbi (framebuffer image viewer)
    try:
        subprocess.run(["sudo", "killall", "fbi"], capture_output=True)
        result = subprocess.run(
            ["sudo", "fbi", "-T", "1", "-d", "/dev/fb0", "-a", "--noverbose", png_path],
            capture_output=True, text=True, timeout=5
        )
        if result.returncode == 0:
            print("  ✓ Displayed via fbi on /dev/fb0")
            return True
        else:
            print(f"  fbi stderr: {result.stderr.strip()}")
    except (FileNotFoundError, subprocess.TimeoutExpired) as e:
        print(f"  fbi not available: {e}")

    # Fallback: write raw pixels to framebuffer
    try:
        from PIL import Image
        img = Image.open(png_path).convert("RGBA")

        with open("/sys/class/graphics/fb0/virtual_size") as f:
            fb_w, fb_h = [int(x) for x in f.read().strip().split(",")]

        img = img.resize((fb_w, fb_h), Image.LANCZOS)
        pixels = img.tobytes("raw", "BGRA")

        with open("/dev/fb0", "wb") as fb:
            fb.write(pixels)

        print(f"  ✓ Displayed via direct framebuffer write ({fb_w}x{fb_h})")
        return True
    except Exception as e:
        print(f"  Framebuffer fallback failed: {e}")

    print(f"  PNG saved at: {png_path}")
    return False


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    print("=" * 50)
    print("  DOF-5 Up/Down Motion Test")
    print("=" * 50)

    print("\n  Generating DOF-5 trace data...")
    print("    - 3 cycles, 2mm travel, 200ms move time")

    data = generate_dof5_updown_trace(
        n_cycles=3,
        travel_mm=2.0,
        move_time_s=0.2,
        dwell_time_s=0.05,
        sample_period_s=0.000512
    )
    time_arr, cmd_pos, act_pos, pos_err, cmd_vel, motor_cur = data
    print(f"    - {len(time_arr)} samples over {time_arr[-1]*1000:.0f} ms")

    csv_path = os.path.join(OUTPUT_DIR, "dof5_updown_test.csv")
    save_trace_csv(csv_path, time_arr, cmd_pos, act_pos, pos_err, cmd_vel, motor_cur)
    print(f"  CSV saved: {csv_path}")

    print("\n  Generating plot...")
    png_path = plot_dof5_trace(time_arr, cmd_pos, act_pos, pos_err, cmd_vel, motor_cur)

    print()
    display_on_screen(png_path)

    print("\n" + "=" * 50)
    print("  Done!")
    print("=" * 50)


if __name__ == "__main__":
    main()
