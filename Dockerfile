FROM debian:trixie
RUN apt-get update && \
apt-get install -y --no-install-recommends \
ca-certificates build-essential cmake git gcc-arm-none-eabi python3-venv && \
rm -rf /var/cache/apt/archives /var/lib/apt/lists/*
RUN python3 -m venv /matplot-venv && . /matplot-venv/bin/activate && pip3 install matplotlib
