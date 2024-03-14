FROM ubuntu:18.04

COPY assets         /app/assets
COPY shaders        /app/shaders
COPY src            /app/src

COPY dev.sh         /app
COPY install.tgz    /app
COPY Makefile       /app

WORKDIR /app

# Install dependencies
RUN apt update -y && \
apt install -y \
    make \
    software-properties-common && \
# Install glslc
add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
apt upgrade libstdc++6 -y && \
tar xvf install.tgz && \
alias glslc="/install/bin/glslc" && \
rm -rf install.tgz && \
# Run dev.sh
chmod +x /app/dev.sh && \
apt autoremove -y && \
apt autoclean -y

ENTRYPOINT ["/app/dev.sh"]