FROM ubuntu:22.04

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
apt autoclean -y && \
useradd \
    -rm \
    -d /home/vox_user \
    -s /bin/bash \
    -g root \
    -G sudo \
    -u 1000 vox_user

USER vox_user

ENTRYPOINT [ "./dev.sh" ]