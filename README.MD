ffmpeg -i input.mp4 -vf "fps=30,scale=-1:240:flags=lanczos,crop=320:in_h:(in_w-320)/2:0" -q:v 11 output.mjpeg

ffmpeg -i input.mp4 -ar 44100 -ac 1 -filter:a loudnorm -filter:a "volume=-5dB" output.aac
