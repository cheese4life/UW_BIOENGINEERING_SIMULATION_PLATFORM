#!/usr/bin/env python3
"""
DOF-5 Interactive Trace Dashboard
===================================
Serves a browser-based dashboard with Plotly.js for trace data from
the MotionSynergy API. Features:
  - Click any point for detailed readout
  - Scroll-wheel zoom / pinch zoom (magnifying)
  - Drag-to-pan scrub across time axis
  - Box zoom & lasso select
  - Crosshair cursor with live coordinate readout
  - Range slider scrub bar below plots
  - Hover tooltip with all values at a time point
  - Export to PNG button

Usage:
  python3 dof5_interactive.py [csv_path] [--port PORT]
  Then open http://<pi-ip>:8050 in any browser on your network.
"""

import csv
import json
import os
import sys
import argparse
from http.server import HTTPServer, SimpleHTTPRequestHandler
from urllib.parse import urlparse, parse_qs

DEFAULT_CSV = os.path.expanduser("~/dof_dashboard/output/dof5_real_trace.csv")
DEFAULT_PORT = 8050


def load_csv(path):
    """Load trace CSV and return {headers, columns} as JSON-ready dict."""
    with open(path, "r") as f:
        reader = csv.reader(f)
        headers = [h.strip() for h in next(reader)]
        columns = {h: [] for h in headers}
        for row in reader:
            for i, h in enumerate(headers):
                try:
                    columns[h].append(float(row[i]))
                except (ValueError, IndexError):
                    columns[h].append(None)
    return {"headers": headers, "columns": columns, "file": os.path.basename(path)}


def build_html(trace_data):
    """Build the full interactive dashboard HTML with embedded Plotly.js."""
    headers = trace_data["headers"]
    columns = trace_data["columns"]
    filename = trace_data["file"]

    time_key = headers[0]  # "Period (s)"
    signal_keys = headers[1:]  # remaining channels

    # Prepare per-channel color scheme
    colors = ["#2196F3", "#FF5722", "#4CAF50", "#9C27B0", "#FF9800", "#00BCD4"]

    # Build Plotly trace JSON for each signal
    traces_json = []
    for i, key in enumerate(signal_keys):
        traces_json.append({
            "x": columns[time_key],
            "y": columns[key],
            "type": "scatter",
            "mode": "lines",
            "name": key,
            "line": {"color": colors[i % len(colors)], "width": 1.5},
            "yaxis": f"y{i+1}" if i > 0 else "y",
            "hovertemplate": f"<b>{key}</b><br>Time: %{{x:.4f}} s<br>Value: %{{y:.4f}}<extra></extra>",
        })

    n_signals = len(signal_keys)

    # Build subplot y-axis layout (stacked vertically)
    y_domains = []
    gap = 0.03
    h = (1.0 - gap * (n_signals - 1)) / n_signals
    for i in range(n_signals):
        top = 1.0 - i * (h + gap)
        bottom = top - h
        y_domains.append([round(bottom, 4), round(top, 4)])

    layout = {
        "title": {
            "text": f"DOF-5 Trace: {filename}",
            "font": {"size": 18, "color": "#E0E0E0"},
        },
        "paper_bgcolor": "#1a1a2e",
        "plot_bgcolor": "#16213e",
        "font": {"color": "#E0E0E0", "family": "monospace"},
        "hovermode": "x unified",
        "dragmode": "zoom",
        "showlegend": True,
        "legend": {
            "orientation": "h",
            "x": 0.5,
            "xanchor": "center",
            "y": 1.08,
            "font": {"size": 12},
        },
        "xaxis": {
            "title": time_key,
            "domain": [0, 1],
            "gridcolor": "#2a3a5e",
            "zerolinecolor": "#3a4a6e",
            "rangeslider": {"visible": True, "thickness": 0.08},
            "showspikes": True,
            "spikemode": "across",
            "spikethickness": 1,
            "spikecolor": "#aaa",
            "spikedash": "dot",
        },
        "margin": {"l": 70, "r": 30, "t": 60, "b": 40},
    }

    for i, key in enumerate(signal_keys):
        yname = f"yaxis{i+1}" if i > 0 else "yaxis"
        layout[yname] = {
            "title": {"text": key, "font": {"size": 11}},
            "domain": y_domains[i],
            "gridcolor": "#2a3a5e",
            "zerolinecolor": "#3a4a6e",
            "anchor": "x" if i == 0 else "free",
            "side": "left",
        }
        if i > 0:
            layout[yname]["overlaying"] = None
            layout[yname]["position"] = 0

    # For stacked subplots, each signal needs its own xaxis reference too
    # Rebuild as proper subplots using Plotly subplot approach in JS
    traces_json_str = json.dumps(traces_json)
    n_samples = len(columns[time_key])
    time_data = json.dumps(columns[time_key])
    signal_data = {k: json.dumps(columns[k]) for k in signal_keys}

    html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>DOF-5 Interactive Trace Dashboard</title>
<script src="https://cdn.plot.ly/plotly-2.35.0.min.js"></script>
<style>
  * {{ margin: 0; padding: 0; box-sizing: border-box; }}
  body {{
    background: #0f0f23;
    color: #e0e0e0;
    font-family: 'Courier New', monospace;
    overflow-x: hidden;
  }}
  #header {{
    background: #1a1a2e;
    padding: 10px 20px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    border-bottom: 2px solid #2196F3;
  }}
  #header h1 {{
    font-size: 16px;
    color: #2196F3;
  }}
  #header .info {{
    font-size: 12px;
    color: #888;
  }}
  #readout {{
    background: #16213e;
    padding: 8px 20px;
    font-size: 13px;
    display: flex;
    gap: 30px;
    flex-wrap: wrap;
    border-bottom: 1px solid #2a3a5e;
    min-height: 36px;
    align-items: center;
  }}
  #readout .val {{
    color: #4fc3f7;
    font-weight: bold;
  }}
  #readout .label {{
    color: #888;
  }}
  #plots {{
    width: 100%;
    height: calc(100vh - 90px);
  }}
  .controls {{
    display: flex;
    gap: 10px;
    align-items: center;
  }}
  .controls button {{
    background: #2a3a5e;
    color: #e0e0e0;
    border: 1px solid #4a5a7e;
    padding: 4px 12px;
    cursor: pointer;
    font-family: monospace;
    font-size: 12px;
    border-radius: 3px;
  }}
  .controls button:hover {{
    background: #3a4a6e;
  }}
  .controls button.active {{
    background: #2196F3;
    border-color: #2196F3;
  }}
</style>
</head>
<body>

<div id="header">
  <h1>DOF-5 MotionSynergy Trace &mdash; {filename}</h1>
  <div class="controls">
    <button id="btnZoom" class="active" onclick="setMode('zoom')">&#128269; Zoom</button>
    <button id="btnPan" onclick="setMode('pan')">&#9995; Pan/Scrub</button>
    <button id="btnReset" onclick="resetView()">&#8634; Reset</button>
    <span class="info">{n_samples} samples &bull; {n_signals} channels</span>
  </div>
</div>

<div id="readout">
  <span class="label">Click or hover on plot for details</span>
</div>

<div id="plots"></div>

<script>
const time = {time_data};
const signals = {{
"""
    for i, key in enumerate(signal_keys):
        comma = "," if i < len(signal_keys) - 1 else ""
        html += f'  "{key}": {signal_data[key]}{comma}\n'

    html += f"""}};
const signalNames = {json.dumps(signal_keys)};
const colors = {json.dumps(colors[:n_signals])};

// Build stacked subplots
const traces = [];
const layout = {{
  paper_bgcolor: '#0f0f23',
  plot_bgcolor: '#16213e',
  font: {{ color: '#e0e0e0', family: 'Courier New, monospace', size: 11 }},
  showlegend: false,
  dragmode: 'zoom',
  margin: {{ l: 65, r: 20, t: 8, b: 45 }},
  hovermode: 'x unified',
}};

const n = signalNames.length;
const gap = 0.04;
const h = (1.0 - gap * (n - 1)) / n;

for (let i = 0; i < n; i++) {{
  const key = signalNames[i];
  const yIdx = i + 1;
  const xIdx = i + 1;
  const xName = i === 0 ? 'x' : 'x' + xIdx;
  const yName = i === 0 ? 'y' : 'y' + yIdx;
  const xAxisKey = i === 0 ? 'xaxis' : 'xaxis' + xIdx;
  const yAxisKey = i === 0 ? 'yaxis' : 'yaxis' + yIdx;

  const top = 1.0 - i * (h + gap);
  const bottom = top - h;

  traces.push({{
    x: time,
    y: signals[key],
    type: 'scatter',
    mode: 'lines',
    name: key,
    line: {{ color: colors[i], width: 1.5 }},
    xaxis: xName,
    yaxis: yName,
    hovertemplate: '<b>' + key + '</b>: %{{y:.4f}}<extra></extra>',
  }});

  // Also add a thin scatter trace for click detection
  traces.push({{
    x: time,
    y: signals[key],
    type: 'scatter',
    mode: 'markers',
    marker: {{ size: 3, color: colors[i], opacity: 0 }},
    xaxis: xName,
    yaxis: yName,
    showlegend: false,
    hoverinfo: 'skip',
  }});

  const isBottom = (i === n - 1);
  layout[xAxisKey] = {{
    domain: [0, 1],
    anchor: yName,
    gridcolor: '#2a3a5e',
    zerolinecolor: '#3a4a6e',
    showticklabels: isBottom,
    showspikes: true,
    spikemode: 'across',
    spikethickness: 1,
    spikecolor: '#666',
    spikedash: 'dot',
    matches: i === 0 ? undefined : 'x',
  }};

  if (isBottom) {{
    layout[xAxisKey].title = {{ text: 'Time (s)', font: {{ size: 12 }} }};
    layout[xAxisKey].rangeslider = {{ visible: true, thickness: 0.06 }};
  }}

  layout[yAxisKey] = {{
    domain: [Math.round(bottom * 1000)/1000, Math.round(top * 1000)/1000],
    anchor: xName,
    title: {{ text: key.split(' (')[0], font: {{ size: 11, color: colors[i] }} }},
    gridcolor: '#2a3a5e',
    zerolinecolor: '#3a4a6e',
  }};
}}

const config = {{
  responsive: true,
  displayModeBar: true,
  modeBarButtonsToAdd: ['toggleSpikelines'],
  modeBarButtonsToRemove: ['lasso2d'],
  displaylogo: false,
  toImageButtonOptions: {{
    format: 'png',
    filename: 'dof5_trace',
    width: 1600,
    height: 900,
  }},
  scrollZoom: true,
}};

Plotly.newPlot('plots', traces, layout, config);

// Click handler — show detailed readout
const plotDiv = document.getElementById('plots');
const readout = document.getElementById('readout');

plotDiv.on('plotly_click', function(data) {{
  if (!data.points.length) return;
  const pt = data.points[0];
  const tIdx = pt.pointIndex;
  const t = time[tIdx];

  let html = '<span class="label">t=</span><span class="val">' + t.toFixed(6) + ' s</span>';
  html += '<span class="label">&nbsp;&nbsp;idx=</span><span class="val">' + tIdx + '</span>';
  for (let i = 0; i < signalNames.length; i++) {{
    const key = signalNames[i];
    const v = signals[key][tIdx];
    const short = key.split(' (')[0];
    const unit = key.match(/\\((.+)\\)/);
    const unitStr = unit ? ' ' + unit[1] : '';
    html += '<span class="label">&nbsp;&nbsp;' + short + '=</span>';
    html += '<span class="val">' + (v !== null ? v.toFixed(4) : 'N/A') + unitStr + '</span>';
  }}
  readout.innerHTML = html;
}});

// Hover handler — live crosshair readout
plotDiv.on('plotly_hover', function(data) {{
  if (!data.points.length) return;
  const pt = data.points[0];
  const tIdx = pt.pointIndex;
  if (tIdx === undefined || tIdx < 0 || tIdx >= time.length) return;
  const t = time[tIdx];

  let html = '<span class="label">t=</span><span class="val">' + t.toFixed(6) + ' s</span>';
  for (let i = 0; i < signalNames.length; i++) {{
    const key = signalNames[i];
    const v = signals[key][tIdx];
    const short = key.split(' (')[0];
    html += '<span class="label">&nbsp;&nbsp;' + short + '=</span>';
    html += '<span class="val">' + (v !== null ? v.toFixed(4) : '--') + '</span>';
  }}
  readout.innerHTML = html;
}});

// Mode toggles
function setMode(mode) {{
  Plotly.relayout('plots', {{ dragmode: mode }});
  document.getElementById('btnZoom').classList.toggle('active', mode === 'zoom');
  document.getElementById('btnPan').classList.toggle('active', mode === 'pan');
}}

function resetView() {{
  // Reset all x-axes to full range
  const update = {{}};
  for (let i = 0; i < n; i++) {{
    const xKey = i === 0 ? 'xaxis.range' : 'xaxis' + (i+1) + '.range';
    update[xKey] = [time[0], time[time.length - 1]];
    const yKey = i === 0 ? 'yaxis.autorange' : 'yaxis' + (i+1) + '.autorange';
    update[yKey] = true;
  }}
  Plotly.relayout('plots', update);
}}

// Keyboard shortcuts
document.addEventListener('keydown', function(e) {{
  if (e.key === 'z') setMode('zoom');
  if (e.key === 'p') setMode('pan');
  if (e.key === 'r') resetView();
}});
</script>
</body>
</html>"""
    return html


class DashboardHandler(SimpleHTTPRequestHandler):
    """HTTP handler serving the interactive dashboard."""
    html_content = ""

    def do_GET(self):
        parsed = urlparse(self.path)
        if parsed.path == "/" or parsed.path == "/index.html":
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", len(self.html_content.encode()))
            self.end_headers()
            self.wfile.write(self.html_content.encode())
        elif parsed.path == "/api/data":
            # JSON endpoint for raw data
            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            data = json.dumps(DashboardHandler.trace_data)
            self.send_header("Content-Length", len(data.encode()))
            self.end_headers()
            self.wfile.write(data.encode())
        else:
            self.send_error(404)

    def log_message(self, format, *args):
        # Quieter logging
        pass


def main():
    parser = argparse.ArgumentParser(description="DOF-5 Interactive Trace Dashboard")
    parser.add_argument("csv", nargs="?", default=DEFAULT_CSV, help="Path to trace CSV")
    parser.add_argument("--port", type=int, default=DEFAULT_PORT, help="HTTP port (default: 8050)")
    args = parser.parse_args()

    if not os.path.exists(args.csv):
        sys.exit(f"CSV not found: {args.csv}")

    print(f"  Loading trace data from {args.csv}...")
    trace_data = load_csv(args.csv)
    n = len(trace_data["columns"][trace_data["headers"][0]])
    print(f"  {n} samples, {len(trace_data['headers'])-1} channels")

    print(f"  Building interactive dashboard...")
    html = build_html(trace_data)

    DashboardHandler.html_content = html
    DashboardHandler.trace_data = trace_data

    server = HTTPServer(("0.0.0.0", args.port), DashboardHandler)
    print(f"")
    print(f"  =====================================================")
    print(f"  DOF-5 Interactive Trace Dashboard")
    print(f"  =====================================================")
    print(f"  Open in browser: http://localhost:{args.port}")
    print(f"  From network:    http://172.28.22.215:{args.port}")
    print(f"  =====================================================")
    print(f"  Controls:")
    print(f"    Scroll wheel  = zoom in/out")
    print(f"    Click + drag  = zoom box (Zoom mode) / pan (Scrub mode)")
    print(f"    Click point   = detailed readout in header bar")
    print(f"    Range slider  = scrub across full timeline")
    print(f"    Double-click  = reset zoom")
    print(f"    Keys: Z=zoom, P=pan, R=reset")
    print(f"  =====================================================")
    print(f"  Ctrl+C to stop")
    print(f"")

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n  Shutting down dashboard server.")
        server.shutdown()


if __name__ == "__main__":
    main()
