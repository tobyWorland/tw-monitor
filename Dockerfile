FROM debian:latest
RUN apt-get update && \
apt-get install -y --no-install-recommends \
ca-certificates build-essential cmake git gcc-arm-none-eabi && \
rm -rf /var/cache/apt/{archives,lists/*}
RUN update-ca-certificates

