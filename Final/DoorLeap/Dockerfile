FROM ubuntu:22.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    libgl1-mesa-dev \
    python3 \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*
ENV QT_ROOT=/opt/qt6
ENV PATH=$QT_ROOT/bin:$PATH
ENV LD_LIBRARY_PATH=$QT_ROOT/lib:$LD_LIBRARY_PATH
WORKDIR /tmp
RUN wget https://download.qt.io/archive/qt/6.8/6.8.3/single/qt-everywhere-src-6.8.3.tar.xz
RUN tar xf qt-everywhere-src-6.8.3.tar.xz && \
    mkdir qt-everywhere-src-6.8.3/build && \
    cd qt-everywhere-src-6.8.3/build && \
    cmake .. -GNinja -DCMAKE_INSTALL_PREFIX=$QT_ROOT -DCMAKE_BUILD_TYPE=Release && \
    ninja && ninja install
WORKDIR /app
COPY . .
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_PREFIX_PATH=$QT_ROOT && \
    make
ENV DISPLAY=:0
CMD ["./build/DoorLeap"]