#!/usr/bin/env python3
"""Stepper motor control UI for Raspberry Pi 5 with 7" touchscreen.
   Runs over X server via xinit (same approach as GreenStack).
   Pure black background, white text and outlines only.
   Drives a NEMA stepper via TMC2209 (STEP=GPIO12, DIR=GPIO20, EN=GPIO16).
"""

import tkinter as tk
import threading
import time
import sys

import RPi.GPIO as GPIO

# ── GPIO pins (BCM) ────────────────────────────────────────────
STEP_PIN = 12
DIR_PIN  = 20
EN_PIN   = 16

# ── Mechanical config (from stepper_test.py) ────────────────────
STEPS_PER_REV = 200
MICROSTEPS    = 8
LEAD_MM       = 8.0
STEPS_PER_MM  = (STEPS_PER_REV * MICROSTEPS) / LEAD_MM

# ── Movement defaults ──────────────────────────────────────────
JOG_SPEED_MM_S = 10.0                       # mm/s while button held
STEP_DELAY     = 1.0 / (JOG_SPEED_MM_S * STEPS_PER_MM * 2)


def gpio_setup():
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(STEP_PIN, GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(DIR_PIN,  GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(EN_PIN,   GPIO.OUT, initial=GPIO.HIGH)   # HIGH = disabled


def gpio_cleanup():
    GPIO.output(EN_PIN, GPIO.HIGH)
    GPIO.cleanup()


class StepperUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Stepper Control")
        self.root.geometry("800x480")
        self.root.attributes("-fullscreen", True)
        self.root.configure(bg="black")
        self.root.config(cursor="none")

        # Hide cursor globally
        self.root.bind_all("<Motion>", lambda e: None)

        # Stepper jog state
        self._jogging = False
        self._jog_thread = None
        self._profile_running = False

        self._build_ui()

    # ── button factory ──────────────────────────────────────────
    def _make_button(self, parent, text):
        """Create a black/white button.  Press/release handled manually."""
        btn = tk.Label(
            parent,
            text=text,
            font=("Arial", 26, "bold"),
            fg="white",
            bg="black",
            highlightbackground="white",
            highlightcolor="white",
            highlightthickness=2,
            bd=0,
            cursor="none",
        )
        return btn

    def _bind_tap(self, widget, on_press, on_release=None):
        """Bind press+release with proper visual feedback."""
        def _press(e):
            widget.configure(bg="white", fg="black")
            if on_press:
                on_press()

        def _release(e):
            widget.configure(bg="black", fg="white")
            if on_release:
                on_release()

        widget.bind("<ButtonPress-1>",   _press)
        widget.bind("<ButtonRelease-1>", _release)

    # ── layout ──────────────────────────────────────────────────
    def _build_ui(self):
        main = tk.Frame(self.root, bg="black", cursor="none")
        main.pack(expand=True, fill=tk.BOTH)

        # Left frame — profiles (60% width)
        left = tk.Frame(main, bg="black", cursor="none")
        left.place(relx=0, rely=0, relwidth=0.6, relheight=1.0)

        for i in range(4):
            btn = self._make_button(left, f"Profile {i + 1}")
            btn.place(
                relx=0.03, rely=i * 0.25 + 0.015,
                relwidth=0.94, relheight=0.22,
            )
            n = i + 1
            self._bind_tap(btn, on_press=lambda n=n: self.on_profile(n))

        # Right frame — UP / DOWN (40% width)
        right = tk.Frame(main, bg="black", cursor="none")
        right.place(relx=0.6, rely=0, relwidth=0.4, relheight=1.0)

        up_btn = self._make_button(right, "▲  UP")
        up_btn.place(relx=0.04, rely=0.015, relwidth=0.92, relheight=0.47)
        self._bind_tap(up_btn,
                       on_press=lambda: self._start_jog(1),
                       on_release=self._stop_jog)

        down_btn = self._make_button(right, "▼  DOWN")
        down_btn.place(relx=0.04, rely=0.51, relwidth=0.92, relheight=0.47)
        self._bind_tap(down_btn,
                       on_press=lambda: self._start_jog(0),
                       on_release=self._stop_jog)

    # ── stepper jog (runs in background thread) ────────────────
    def _start_jog(self, direction):
        """Begin continuous stepping in the given direction."""
        if self._jogging or self._profile_running:
            return
        self._jogging = True
        GPIO.output(EN_PIN, GPIO.LOW)           # enable driver
        GPIO.output(DIR_PIN, GPIO.HIGH if direction else GPIO.LOW)
        time.sleep(0.001)

        def _step_loop():
            while self._jogging:
                GPIO.output(STEP_PIN, GPIO.HIGH)
                time.sleep(STEP_DELAY)
                GPIO.output(STEP_PIN, GPIO.LOW)
                time.sleep(STEP_DELAY)
            GPIO.output(EN_PIN, GPIO.HIGH)      # disable when done

        self._jog_thread = threading.Thread(target=_step_loop, daemon=True)
        self._jog_thread.start()

    def _stop_jog(self):
        """Stop the jog loop."""
        self._jogging = False

    # ── move N steps at a given delay ──────────────────────────
    def _move_steps(self, steps, delay):
        """Blocking: pulse STEP pin for N steps at the given half-period delay."""
        for _ in range(steps):
            if not self._profile_running:
                return
            GPIO.output(STEP_PIN, GPIO.HIGH)
            time.sleep(delay)
            GPIO.output(STEP_PIN, GPIO.LOW)
            time.sleep(delay)

    # ── profile routines ───────────────────────────────────────
    def _run_profile(self, number):
        """Dispatch to the right profile routine in a background thread."""
        if self._profile_running or self._jogging:
            return
        self._profile_running = True

        def _worker():
            GPIO.output(EN_PIN, GPIO.LOW)       # enable driver
            time.sleep(0.01)
            try:
                if number == 1:
                    self._profile_bounce(dist_mm=10, speed_mm_s=40.0, duration_s=5)
                elif number == 2:
                    self._profile_bounce(dist_mm=20, speed_mm_s=5.0, duration_s=5)
                elif number == 3:
                    self._profile_precision(dist_mm=20, speed_mm_s=1.0)
                else:
                    print(f"Profile {number}: not configured")
            finally:
                GPIO.output(EN_PIN, GPIO.HIGH)  # disable driver
                self._profile_running = False

        t = threading.Thread(target=_worker, daemon=True)
        t.start()

    def _profile_bounce(self, dist_mm, speed_mm_s, duration_s):
        """Bounce back and forth dist_mm at speed_mm_s for duration_s seconds."""
        steps = int(dist_mm * STEPS_PER_MM)
        delay = 1.0 / (speed_mm_s * STEPS_PER_MM * 2)
        direction = 1
        end_time = time.monotonic() + duration_s

        while self._profile_running and time.monotonic() < end_time:
            GPIO.output(DIR_PIN, GPIO.HIGH if direction else GPIO.LOW)
            time.sleep(0.001)
            self._move_steps(steps, delay)
            direction ^= 1             # flip direction each pass

    def _profile_precision(self, dist_mm, speed_mm_s):
        """Single slow precision move of dist_mm, then return."""
        steps = int(dist_mm * STEPS_PER_MM)
        delay = 1.0 / (speed_mm_s * STEPS_PER_MM * 2)

        # Move forward
        GPIO.output(DIR_PIN, GPIO.HIGH)
        time.sleep(0.001)
        self._move_steps(steps, delay)

        time.sleep(0.25)

        # Move back
        GPIO.output(DIR_PIN, GPIO.LOW)
        time.sleep(0.001)
        self._move_steps(steps, delay)

    # ── profile callback ───────────────────────────────────────
    def on_profile(self, number):
        print(f"Profile {number} selected")
        self._run_profile(number)

    # ── cleanup ─────────────────────────────────────────────────
    def cleanup(self):
        self._jogging = False
        self._profile_running = False
        if self._jog_thread:
            self._jog_thread.join(timeout=0.5)
        gpio_cleanup()


if __name__ == "__main__":
    gpio_setup()
    root = tk.Tk()
    app = StepperUI(root)

    def on_close():
        app.cleanup()
        root.destroy()

    root.protocol("WM_DELETE_WINDOW", on_close)

    try:
        root.mainloop()
    finally:
        app.cleanup()
