FROM ubuntu:18.04

WORKDIR /app

# Install dependencies
RUN apt update -y && \
apt install -y \
    make \
# Install glslc
    software-properties-common && \
add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
apt upgrade libstdc++6 -y && \
# Cleanup
apt autoremove -y && \
apt autoclean -y

CMD ["tail", "-f", "/dev/null"]