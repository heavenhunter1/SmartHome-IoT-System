import tkinter as tk
import threading
import requests

# =====================================================
# CONFIG
# =====================================================
ESP32_IP = ESP32_IP = "YOUR_ESP32_IP"
UPDATE_INTERVAL_MS = 1000

# =====================================================
# COLORS
# =====================================================
BG_COLOR = "#0f172a"
CARD_COLOR = "#1e293b"
TEXT_COLOR = "#e5e7eb"
MUTED_TEXT = "#94a3b8"

GREEN = "#22c55e"
RED = "#ef4444"
BLUE = "#3b82f6"
YELLOW = "#eab308"

BUTTON_ON = "#2563eb"
BUTTON_OFF = "#475569"


# =====================================================
# NETWORK
# =====================================================
def send_request(cmd):
    try:
        response = requests.get(
            f"http://{ESP32_IP}/{cmd}",
            timeout=1
        )
        print("Response:", response.text)

    except Exception as e:
        print("ESP32 Error:", e)


def handle_command(cmd):
    print("Command:", cmd)

    threading.Thread(
        target=send_request,
        args=(cmd,),
        daemon=True
    ).start()


def update_status():
    try:
        response = requests.get(
            f"http://{ESP32_IP}/status",
            timeout=1
        )

        data = response.json()

        set_light_ui(data["light"])
        set_fan_ui(data["fan"])
        set_auto_light_ui(data["autoLight"])
        set_auto_fan_ui(data["autoFan"])

        if "temp" in data and "humidity" in data:
            set_environment_ui(data["temp"], data["humidity"])

        connection_label.config(
            text="ESP32 Connected",
            fg=GREEN
        )

    except Exception as e:
        print("Status Error:", e)
        connection_label.config(
            text="ESP32 Disconnected",
            fg=RED
        )


def auto_update():
    update_status()
    app.after(UPDATE_INTERVAL_MS, auto_update)


# =====================================================
# UI UPDATE FUNCTIONS
# =====================================================
def set_light_ui(is_on):
    if is_on:
        light_dot.config(bg=GREEN)
        light_status.config(text="ON", fg=GREEN)
        light_btn.config(text="Turn OFF Light", bg=BUTTON_OFF)
    else:
        light_dot.config(bg=RED)
        light_status.config(text="OFF", fg=RED)
        light_btn.config(text="Turn ON Light", bg=BUTTON_ON)


def set_fan_ui(is_on):
    if is_on:
        fan_dot.config(bg=GREEN)
        fan_status.config(text="ON", fg=GREEN)
        fan_btn.config(text="Turn OFF Fan", bg=BUTTON_OFF)
    else:
        fan_dot.config(bg=RED)
        fan_status.config(text="OFF", fg=RED)
        fan_btn.config(text="Turn ON Fan", bg=BUTTON_ON)


def set_auto_light_ui(is_auto):
    if is_auto:
        auto_light_btn.config(
            text="Auto",
            bg=GREEN,
            activebackground="#16a34a"
        )
    else:
        auto_light_btn.config(
            text="Manual",
            bg=YELLOW,
            activebackground="#ca8a04"
        )


def set_auto_fan_ui(is_auto):
    if is_auto:
        auto_fan_btn.config(
            text="Auto",
            bg=GREEN,
            activebackground="#16a34a"
        )
    else:
        auto_fan_btn.config(
            text="Manual",
            bg=YELLOW,
            activebackground="#ca8a04"
        )


def set_environment_ui(temp_c, humidity):
    temp_f = temp_c * 9 / 5 + 32

    temp_value.config(
        text=f"{temp_c:.1f}°C / {temp_f:.1f}°F"
    )

    humidity_value.config(
        text=f"{humidity:.1f}%"
    )

    if temp_c < 22:
        temp_value.config(fg=BLUE)
        temp_note.config(text="Cool", fg=BLUE)
    elif temp_c <= 30:
        temp_value.config(fg=GREEN)
        temp_note.config(text="Comfortable", fg=GREEN)
    else:
        temp_value.config(fg=RED)
        temp_note.config(text="Hot", fg=RED)


# =====================================================
# BUTTON COMMANDS
# =====================================================
def toggle_light():
    if light_status.cget("text") == "ON":
        set_light_ui(False)
        handle_command("light_off")
    else:
        set_light_ui(True)
        handle_command("light_on")


def toggle_auto_light():
    if auto_light_btn.cget("text") == "Auto":
        set_auto_light_ui(False)
        handle_command("auto_light_off")
    else:
        set_auto_light_ui(True)
        handle_command("auto_light_on")


def toggle_fan():
    if fan_status.cget("text") == "ON":
        set_fan_ui(False)
        handle_command("fan_off")
    else:
        set_fan_ui(True)
        handle_command("fan_on")


def toggle_auto_fan():
    if auto_fan_btn.cget("text") == "Auto":
        set_auto_fan_ui(False)
        handle_command("auto_fan_off")
    else:
        set_auto_fan_ui(True)
        handle_command("auto_fan_on")


# =====================================================
# UI HELPERS
# =====================================================
def create_card(parent):
    frame = tk.Frame(
        parent,
        bg=CARD_COLOR,
        padx=16,
        pady=14
    )
    frame.pack(fill="x", padx=16, pady=8)
    return frame


def create_status_dot(parent, color):
    return tk.Label(
        parent,
        text="",
        width=2,
        height=1,
        bg=color
    )


def create_mode_button(parent, text, command):
    return tk.Button(
        parent,
        text=text,
        font=("Segoe UI", 9, "bold"),
        width=8,
        bg=YELLOW,
        fg="white",
        activebackground="#ca8a04",
        activeforeground="white",
        relief="flat",
        command=command
    )


def create_main_button(parent, text, command):
    return tk.Button(
        parent,
        text=text,
        font=("Segoe UI", 10, "bold"),
        bg=BUTTON_ON,
        fg="white",
        activebackground="#1d4ed8",
        activeforeground="white",
        relief="flat",
        command=command
    )


# =====================================================
# SCROLLABLE LAYOUT
# =====================================================
def build_scrollable_area():
    global main_canvas, content_frame, content_window

    main_canvas = tk.Canvas(
        app,
        bg=BG_COLOR,
        highlightthickness=0
    )
    main_canvas.pack(side="left", fill="both", expand=True)

    scrollbar = tk.Scrollbar(
        app,
        orient="vertical",
        command=main_canvas.yview
    )
    scrollbar.pack(side="right", fill="y")

    main_canvas.configure(yscrollcommand=scrollbar.set)

    content_frame = tk.Frame(main_canvas, bg=BG_COLOR)

    content_window = main_canvas.create_window(
        (0, 0),
        window=content_frame,
        anchor="nw"
    )

    content_frame.bind("<Configure>", update_scroll_region)
    main_canvas.bind("<Configure>", resize_content_width)
    main_canvas.bind_all("<MouseWheel>", on_mousewheel)


def update_scroll_region(event=None):
    main_canvas.configure(scrollregion=main_canvas.bbox("all"))


def resize_content_width(event):
    main_canvas.itemconfig(content_window, width=event.width)


def on_mousewheel(event):
    main_canvas.yview_scroll(int(-1 * (event.delta / 120)), "units")


# =====================================================
# UI SECTIONS
# =====================================================
def build_header(parent):
    global connection_label

    header = tk.Frame(parent, bg=BG_COLOR)
    header.pack(fill="x", padx=18, pady=(18, 8))

    title_label = tk.Label(
        header,
        text="Smart Home",
        font=("Segoe UI", 23, "bold"),
        bg=BG_COLOR,
        fg=TEXT_COLOR
    )
    title_label.pack(anchor="w")

    subtitle_label = tk.Label(
        header,
        text="ESP32 IoT Control Dashboard",
        font=("Segoe UI", 10),
        bg=BG_COLOR,
        fg=MUTED_TEXT
    )
    subtitle_label.pack(anchor="w", pady=(2, 0))

    connection_label = tk.Label(
        header,
        text="Connecting...",
        font=("Segoe UI", 10, "bold"),
        bg=BG_COLOR,
        fg=YELLOW
    )
    connection_label.pack(anchor="w", pady=(8, 0))


def build_light_card(parent):
    global light_dot, light_status, light_btn, auto_light_btn

    card = create_card(parent)

    top = tk.Frame(card, bg=CARD_COLOR)
    top.pack(fill="x")

    name = tk.Label(
        top,
        text="LIGHT",
        font=("Segoe UI", 10, "bold"),
        bg=CARD_COLOR,
        fg=MUTED_TEXT
    )
    name.pack(side="left")

    light_status = tk.Label(
        top,
        text="OFF",
        font=("Segoe UI", 10, "bold"),
        bg=CARD_COLOR,
        fg=RED
    )
    light_status.pack(side="right")

    middle = tk.Frame(card, bg=CARD_COLOR)
    middle.pack(fill="x", pady=(12, 14))

    light_dot = create_status_dot(middle, RED)
    light_dot.pack(side="left")

    light_text = tk.Label(
        middle,
        text="Main Room Light",
        font=("Segoe UI", 15, "bold"),
        bg=CARD_COLOR,
        fg=TEXT_COLOR
    )
    light_text.pack(side="left", padx=12)

    button_row = tk.Frame(card, bg=CARD_COLOR)
    button_row.pack(fill="x")

    light_btn = create_main_button(
        button_row,
        "Turn ON Light",
        toggle_light
    )
    light_btn.pack(side="left", fill="x", expand=True, ipady=6)

    auto_light_btn = create_mode_button(
        button_row,
        "Auto",
        toggle_auto_light
    )
    auto_light_btn.pack(side="left", padx=(8, 0), ipady=6)


def build_fan_card(parent):
    global fan_dot, fan_status, fan_btn, auto_fan_btn

    card = create_card(parent)

    top = tk.Frame(card, bg=CARD_COLOR)
    top.pack(fill="x")

    name = tk.Label(
        top,
        text="FAN",
        font=("Segoe UI", 10, "bold"),
        bg=CARD_COLOR,
        fg=MUTED_TEXT
    )
    name.pack(side="left")

    fan_status = tk.Label(
        top,
        text="OFF",
        font=("Segoe UI", 10, "bold"),
        bg=CARD_COLOR,
        fg=RED
    )
    fan_status.pack(side="right")

    middle = tk.Frame(card, bg=CARD_COLOR)
    middle.pack(fill="x", pady=(12, 14))

    fan_dot = create_status_dot(middle, RED)
    fan_dot.pack(side="left")

    fan_text = tk.Label(
        middle,
        text="Smart Fan",
        font=("Segoe UI", 15, "bold"),
        bg=CARD_COLOR,
        fg=TEXT_COLOR
    )
    fan_text.pack(side="left", padx=12)

    button_row = tk.Frame(card, bg=CARD_COLOR)
    button_row.pack(fill="x")

    fan_btn = create_main_button(
        button_row,
        "Turn ON Fan",
        toggle_fan
    )
    fan_btn.pack(side="left", fill="x", expand=True, ipady=6)

    auto_fan_btn = create_mode_button(
        button_row,
        "Manual",
        toggle_auto_fan
    )
    auto_fan_btn.pack(side="left", padx=(8, 0), ipady=6)


def build_environment_card(parent):
    global temp_value, temp_note, humidity_value

    card = create_card(parent)

    env_title = tk.Label(
        card,
        text="ENVIRONMENT",
        font=("Segoe UI", 10, "bold"),
        bg=CARD_COLOR,
        fg=MUTED_TEXT
    )
    env_title.pack(anchor="w")

    temp_label = tk.Label(
        card,
        text="Temperature",
        font=("Segoe UI", 11),
        bg=CARD_COLOR,
        fg=MUTED_TEXT
    )
    temp_label.pack(anchor="w", pady=(14, 0))

    temp_value = tk.Label(
        card,
        text="--.-°C / --.-°F",
        font=("Segoe UI", 22, "bold"),
        bg=CARD_COLOR,
        fg=TEXT_COLOR
    )
    temp_value.pack(anchor="w")

    temp_note = tk.Label(
        card,
        text="Waiting for data",
        font=("Segoe UI", 10, "bold"),
        bg=CARD_COLOR,
        fg=YELLOW
    )
    temp_note.pack(anchor="w", pady=(0, 14))

    humidity_label = tk.Label(
        card,
        text="Humidity",
        font=("Segoe UI", 11),
        bg=CARD_COLOR,
        fg=MUTED_TEXT
    )
    humidity_label.pack(anchor="w")

    humidity_value = tk.Label(
        card,
        text="--.-%",
        font=("Segoe UI", 20, "bold"),
        bg=CARD_COLOR,
        fg=BLUE
    )
    humidity_value.pack(anchor="w", pady=(0, 4))


# =====================================================
# APP BUILD
# =====================================================
def build_app():
    build_scrollable_area()

    build_header(content_frame)
    build_light_card(content_frame)
    build_fan_card(content_frame)
    build_environment_card(content_frame)


# =====================================================
# MAIN
# =====================================================
app = tk.Tk()
app.title("Smart Home Dashboard")
app.geometry("420x650")
app.minsize(360, 500)
app.resizable(True, True)
app.configure(bg=BG_COLOR)

build_app()

auto_update()
app.mainloop()
