import wave
import numpy as np

# ---------------- CONFIG ----------------
wav_file = "song.wav"        # Input WAV file
c_file = "song_data.c"       # Output C file
max_samples = 50000          # Limit samples to ~6 seconds at 8kHz (adjust as needed)
invert_for_pushpull = True   # Create second array inverted for push-pull
# ----------------------------------------

# Open WAV file
with wave.open(wav_file, "rb") as wf:
    n_channels = wf.getnchannels()
    sampwidth = wf.getsampwidth()
    framerate = wf.getframerate()
    n_frames = wf.getnframes()

    print(f"Channels: {n_channels}, Sample width: {sampwidth} bytes, Frame rate: {framerate}, Total frames: {n_frames}")

    # Read raw audio frames
    frames = wf.readframes(n_frames)

# Convert to 8-bit unsigned values
if sampwidth == 1:
    # 8-bit PCM already unsigned
    data = np.frombuffer(frames, dtype=np.uint8)
elif sampwidth == 2:
    # 16-bit PCM → scale from [-32768, 32767] to [0, 255]
    data = np.frombuffer(frames, dtype=np.int16)
    data = ((data.astype(np.int32) + 32768) * 255 // 65535).astype(np.uint8)
else:
    raise ValueError(f"Unsupported sample width: {sampwidth} bytes")

# If stereo, take only one channel
if n_channels > 1:
    data = data[::n_channels]

# Limit number of samples if desired
if max_samples:
    data = data[:max_samples]

# Prepare push-pull inverted array
if invert_for_pushpull:
    data_inv = 255 - data
else:
    data_inv = data.copy()

# Write to C file
with open(c_file, "w") as f:
    f.write("// Generated from WAV file for STM32 push-pull PWM\n\n")
    f.write(f"const uint8_t song_pwm1[{len(data)}] = {{\n")
    f.write(", ".join(map(str, data)))
    f.write("\n};\n\n")

    f.write(f"const uint8_t song_pwm2[{len(data_inv)}] = {{\n")
    f.write(", ".join(map(str, data_inv)))
    f.write("\n};\n")

print(f"C arrays written to {c_file}, total samples: {len(data)}")