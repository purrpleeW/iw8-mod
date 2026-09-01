import argparse
import json
import os
import shutil
import subprocess
import tkinter
import tkinter.filedialog
import tkinter.messagebox

wnd_root = tkinter.Tk()
wnd_root.withdraw()

supported_game_versions = [
	"0.01.2.7089334",
	"1.03.0.7209368",
	"1.20.4.7623265",
	"1.38.3.9489393",
	"1.44.0.10435696"
]
mod_dll_name = "XInput9_1_0.dll"

directories = {}
directories_path = os.path.join(os.path.dirname(__file__), "directories.json")

if os.path.exists(directories_path):
	with open(directories_path, "r") as f:
		directories = json.load(f)

for version in supported_game_versions:
	if version not in directories:
		directories[version] = ""

def save_directories():
	global directories
	global directories_path
	with open(directories_path, "w") as f:
		json.dump(directories, f)

argument_parser = argparse.ArgumentParser(description="Call of Duty: Modern Warfare Launcher")

argument_parser.add_argument("--version", type=str, choices=supported_game_versions, help="Version of Modern Warfare to launch",
	default="1.20.4.7623265")
argument_parser.add_argument("--custom-exe", type=str, help="Choose a custom Modern Warfare executable", default="ModernWarfare")

arguments = argument_parser.parse_args()

arg_version = str(arguments.version)
arg_custom_exe = str(arguments.custom_exe)

if not arg_custom_exe.endswith(".exe"):
	arg_custom_exe += ".exe"

selected_directory = directories[arguments.version]
if len(selected_directory) < 1 or not os.path.isdir(selected_directory):
	selected_directory = tkinter.filedialog.askdirectory(title=f"Find game directory of Modern Warfare v{arguments.version}")

if not selected_directory:
	tkinter.messagebox.showinfo("iw8-mod", "No directory chosen, cancelling.")
	exit()

directories[arguments.version] = selected_directory
save_directories()

arg_custom_exe_absolute = os.path.join(selected_directory, arg_custom_exe)
if not os.path.isfile(arg_custom_exe_absolute):
	tkinter.messagebox.showinfo("iw8-mod", f"Couldn't find executable \"{arg_custom_exe_absolute}\".")
	exit()

iw8_mod_dll = os.path.join(os.path.dirname(__file__), "..", "..", "build", "iw8_vs2022", "x64", "client", mod_dll_name)
try:
	shutil.copy(iw8_mod_dll, os.path.join(selected_directory, mod_dll_name))
except FileNotFoundError:
	tkinter.messagebox.showinfo("iw8-mod", f"Couldn't find new {mod_dll_name}, not updating.")
except Exception as e:
	tkinter.messagebox.showinfo("iw8-mod", f"Caught an unknown exception: {e}")

os.chdir(selected_directory)
subprocess.Popen([ arg_custom_exe ])
