#!/usr/bin/env python3
"""
DOF-5 Real API Up/Down Test with Trace Capture
================================================
Uses the actual MotionSynergy API with emulated DOF-5 hardware
to perform up/down moves, capture real trace data, and display
the results on the Pi's external screen.
"""

import os
import sys
import csv
import subprocess
import time

# --- Setup paths BEFORE importing pythonnet ---
API_ROOT = "/home/anton/MotionSynergyAPI"
OUTPUT_DIR = os.path.expanduser("~/dof_dashboard/output")
os.makedirs(OUTPUT_DIR, exist_ok=True)

# Change to API root so it can find SupportFolder, DLLs, etc.
os.chdir(API_ROOT)

# Add API root to Python path and system library path
sys.path.insert(0, API_ROOT)
os.environ["LD_LIBRARY_PATH"] = API_ROOT + ":" + os.environ.get("LD_LIBRARY_PATH", "")

# Set up .NET runtime
os.environ["DOTNET_ROOT"] = os.path.expanduser("~/.dotnet")

# --- Load pythonnet with CoreCLR ---
from pythonnet import load
load("coreclr")

import clr
from System.Collections.Generic import List
from System import String

# Load the MotionSynergy API assembly
clr.AddReference("MotionSynergyAPI")
from MotionSynergyAPI import MotionSynergyAPINative, InstrumentSettings, LogLevel

# --- Matplotlib for plotting (headless) ---
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def setup_api():
    """Configure and initialize the MotionSynergy API with emulated DOF-5."""
    print("  Creating MotionSynergy API instance...")
    api = MotionSynergyAPINative()

    settings = InstrumentSettings()
    settings.ApplicationVersionString = "1.0"
    settings.SupportFolder = "./SupportFolder"
    settings.ProgramDataFolder = "./ProgramDataFolder"
    settings.ConfigurationFilename = "Instrument.cfg"

    print("  Configuring API...")
    result = api.Configure(settings).Result
    if not result.Success:
        sys.exit(f"Configuration failed: {result}")

    print("  Initializing (connect + home emulated DOF-5)...")
    result = api.Initialize().Result
    if not result.Success:
        sys.exit(f"Initialization failed: {result}")

    print("  API initialized successfully!")
    return api


def wait_result(task_or_result):
    """Handle both Task<InstrumentResult> and InstrumentResult returns."""
    if hasattr(task_or_result, 'Result'):
        return task_or_result.Result
    return task_or_result


def perform_traced_moves(api):
    """Perform up/down moves on the DOF-5 with trace capture enabled."""
    product = api.GetFirstProduct()
    axis_names = product.AxisNames
    axis = None
    for a in api.AxisList:
        if a.Name == axis_names[0]:
            axis = a
            break

    if axis is None:
        sys.exit("Could not find axis!")

    axis_name = axis.Name
    print(f"  Axis: {axis_name}")
    pos_result = axis.GetCommandedPosition()
    print(f"  Position result: {pos_result}")

    # Select the standard move profile (returns InstrumentResult directly)
    r = axis.SelectMoveProfile(f"{axis_name}StandardMoveProfile")
    print(f"  SelectMoveProfile: {r}")

    # Enable trace capture
    # NEnableTraceCapture(TraceTrigger trigger, String traceProfile)
    # TraceTrigger.Immediate = 0, OnMotionStart = 1
    from MotionSynergyAPI import TraceTrigger
    print("  Enabling trace capture (MoveTraceProfile)...")
    r = axis.NEnableTraceCapture(TraceTrigger.OnMotionStart, "MoveTraceProfile").Result
    print(f"  NEnableTraceCapture: {r}")

    # Move to home first (returns Task<InstrumentResult>)
    print("  Moving to Home (0 mm)...")
    r = axis.MoveAbsolute(0.0).Result
    print(f"  MoveAbsolute(0): {r}")

    # Perform up/down moves (within DOF-5 ±2.5mm range)
    n_cycles = 3
    move_distance = 2.0  # mm
    print(f"  Performing {n_cycles} up/down cycles, {move_distance}mm travel...")

    for i in range(n_cycles):
        # Move up (Task — block with .Result)
        axis.MoveAbsolute(move_distance).Result
        # Move down
        axis.MoveAbsolute(0.0).Result
        print(f"    Cycle {i+1}/{n_cycles} complete")

    # Stop trace capture (returns Task<InstrumentResult>)
    print("  Stopping trace capture...")
    r = axis.StopTraceCapture().Result
    print(f"  StopTraceCapture: {r}")

    # Save trace to CSV
    # SaveTraceCapture(String pathname, Int32 traceCount)
    # traceCount=0 means save all captured traces
    trace_csv = os.path.join(OUTPUT_DIR, "dof5_real_trace.csv")
    print(f"  Saving trace data to {trace_csv}...")
    r = axis.SaveTraceCapture(trace_csv, 0).Result
    print(f"  SaveTraceCapture: {r}")

    return trace_csv


def plot_trace(csv_path):
    """Plot the real trace data, optimized for 800x480 screen."""
    print(f"  Reading trace data from {csv_path}...")

    # Parse CSV
    with open(csv_path, "r") as f:
        reader = csv.reader(f)
        headers = [h.strip() for h in next(reader)]
        columns = [[] for _ in headers]
        for row in reader:
            for i, val in enumerate(row):
                try:
                    columns[i].append(float(val.strip()))
                except (ValueError, IndexError):
                    pass

    if len(columns[0]) == 0:
        print("  WARNING: No data in trace CSV!")
        return None

    print(f"  {len(columns[0])} samples, {len(headers)} channels")

    time_data = np.array(columns[0])
    t_ms = time_data * 1000  # Convert to ms

    n_data_cols = len(headers) - 1
    fig, axes = plt.subplots(n_data_cols, 1, figsize=(8, 4.8), sharex=True,
                              gridspec_kw={"hspace": 0.35})
    if n_data_cols == 1:
        axes = [axes]

    fig.patch.set_facecolor("#0d1117")
    colors = ["#58a6ff", "#f0883e", "#79c0ff", "#d2a8ff", "#7ee787"]

    for ax in axes:
        ax.set_facecolor("#161b22")
        ax.tick_params(colors="#c9d1d9", labelsize=7)
        for spine in ax.spines.values():
            spine.set_color("#30363d")
        ax.grid(True, alpha=0.15, color="#c9d1d9")

    for i in range(n_data_cols):
        ax = axes[i]
        col = np.array(columns[i + 1])
        color = colors[i % len(colors)]
        label = headers[i + 1]

        ax.plot(t_ms[:len(col)], col, color=color, linewidth=0.7)

        # Shorter label for y-axis
        short_label = label.split("(")[0].strip()
        unit = ""
        if "(" in label:
            unit = "(" + label.split("(")[1]
        ax.set_ylabel(f"{short_label}\n{unit}", fontsize=7, color="#c9d1d9")

        # Stats
        rms = np.sqrt(np.mean(col**2))
        peak = np.max(np.abs(col))
        ax.text(0.01, 0.92, f"RMS={rms:.4f}  Peak={peak:.4f}",
                transform=ax.transAxes, fontsize=6, color="#7ee787",
                verticalalignment="top",
                bbox=dict(boxstyle="round,pad=0.2", facecolor="#0d1117", alpha=0.8))

    axes[0].set_title("DOF-5 Real API Trace — Up/Down Motion (Emulated)",
                       fontsize=10, color="#58a6ff", fontweight="bold", pad=8)
    axes[-1].set_xlabel("Time (ms)", fontsize=8, color="#c9d1d9")

    out_png = os.path.join(OUTPUT_DIR, "dof5_real_trace.png")
    fig.savefig(out_png, dpi=100, facecolor=fig.get_facecolor(),
                bbox_inches="tight", pad_inches=0.15)
    plt.close(fig)
    print(f"  Plot saved: {out_png}")
    return out_png


def display_on_screen(png_path):
    """Display on the Pi's external screen."""
    print("  Displaying on screen...")
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
            print(f"  fbi error: {result.stderr.strip()}")
    except Exception as e:
        print(f"  fbi failed: {e}")

    # Fallback: direct framebuffer write
    try:
        from PIL import Image
        img = Image.open(png_path).convert("RGBA")
        with open("/sys/class/graphics/fb0/virtual_size") as f:
            fb_w, fb_h = [int(x) for x in f.read().strip().split(",")]
        img = img.resize((fb_w, fb_h), Image.LANCZOS)
        with open("/dev/fb0", "wb") as fb:
            fb.write(img.tobytes("raw", "BGRA"))
        print(f"  ✓ Displayed via framebuffer ({fb_w}x{fb_h})")
        return True
    except Exception as e:
        print(f"  Framebuffer failed: {e}")

    return False


def main():
    print("=" * 55)
    print("  DOF-5 Real API Up/Down Test (Emulated Hardware)")
    print("=" * 55)
    print()

    # 1. Initialize API
    api = setup_api()

    # 2. Perform moves with trace capture
    print()
    csv_path = perform_traced_moves(api)

    # 3. Shutdown API
    print("\n  Shutting down API...")
    result = api.Shutdown()
    if not result.Success:
        print(f"  Shutdown warning: {result}")
    print("  API shut down.")

    # 4. Plot the trace
    print()
    png_path = plot_trace(csv_path)

    # 5. Display on screen
    if png_path:
        print()
        display_on_screen(png_path)

    print("\n" + "=" * 55)
    print("  Done! This is REAL MotionSynergy API data.")
    print("=" * 55)


if __name__ == "__main__":
    main()
