
FROM ubuntu:latest

# Install the necessary dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    ninja-build \
    python3 \
    python3-pip \
    xxd \
    bsdmainutils \
    64tass \
    cc65 \
    xa65 \ 
    acme \
    zip \
    libsdl2-dev \
    libusb-dev \
    gcc-arm-none-eabi 

# Raspberry Pi Pico SDK
ARG SDK_PATH=/usr/share/pico_sdk
RUN git clone --depth 1 --branch master https://github.com/raspberrypi/pico-sdk $SDK_PATH && \
    cd $SDK_PATH && \
    git submodule update --init

ENV PICO_SDK_PATH=$SDK_PATH

# Picotool installation
#RUN git clone --depth 1 --branch master https://github.com/raspberrypi/picotool.git /home/picotool && \
#    cd /home/picotool && \
#    mkdir build && \
#    cd build && \
#    cmake .. && \
#    make && \
#    cp /home/picotool/build/picotool /bin/picotool && \
#    rm -rf /home/picotool


WORKDIR /tmp


RUN git clone https://bitbucket.org/magli143/exomizer.git
RUN make -C exomizer/src && cp exomizer/src/exomizer /usr/bin  && cp exomizer/src/exobasic /usr/bin 

# Set user and group
ARG user=devel
ARG group=devel
ARG uid=1000
ARG gid=1000
RUN groupadd -g ${gid} ${group}
RUN useradd -m -u ${uid} -g ${gid} -s /bin/bash ${user}


# Switch to non-root user

#ENV PATH=$PATH:$MOS_SDK_PATH/bin

USER ${user}:${group}
WORKDIR /home/${user}

# Set the default command to run when the container starts

CMD ["bash"]
