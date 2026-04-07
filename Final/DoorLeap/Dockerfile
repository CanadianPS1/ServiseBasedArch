# Stage 1 – Qt installation
FROM ubuntu:22.04 AS qt
ARG DEBIAN_FRONTEND=noninteractive
RUN ln -fs /usr/share/zoneinfo/UTC /etc/localtime && \
    apt-get update && apt-get install -y --no-install-recommends \
      curl wget zip unzip build-essential git python3 python3-pip \
      libgl1-mesa-dev libglu1-mesa-dev libfontconfig1-dev libfreetype6-dev \
      libx11-dev libx11-xcb-dev libxkbcommon-dev libxkbcommon-x11-dev \
      libxcb-xinerama0-dev libxcb-xinerama0 libxcb-cursor-dev libxcb-cursor0 \
      libxcb-keysyms1 libxcb-image0 libxcb-icccm4 libxcb-sync1 \
      libxcb-shape0 libxcb-render0 libxcb-render-util0 \
      libxext-dev libxinerama-dev libxi-dev \
      libdbus-1-dev libdbus-1-3 \
      pkg-config && \
    rm -rf /var/lib/apt/lists/*
RUN pip3 install --no-cache-dir aqtinstall
# Install Qt 6.5.3 with QtQuick and Shader Tools modules
RUN aqt install-qt linux desktop 6.5.3 gcc_64 -O /opt/qt \
    -m qtquicktimeline qtshadertools

# Stage 2 – Build application
FROM qt AS builder
ENV PATH=/opt/qt/6.5.3/gcc_64/bin:$PATH
ENV CMAKE_PREFIX_PATH=/opt/qt/6.5.3/gcc_64/lib/cmake
RUN apt-get update && apt-get install -y --no-install-recommends \
      cmake libdbus-1-3 && \
    rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . /app
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)
# Stage 3 – Runtime image
FROM ubuntu:22.04
ARG DEBIAN_FRONTEND=noninteractive
RUN ln -fs /usr/share/zoneinfo/UTC /etc/localtime && \
    apt-get update && apt-get install -y --no-install-recommends \
      libgl1 libglu1-mesa libxcb-xinerama0 libxcb-cursor0 \
      libxkbcommon0 libxkbcommon-x11-0 libx11-6 libxext6 libx11-xcb1 \
      libxcb-shm0 libxcb-icccm4 libxcb-keysyms1 libxcb-image0 libxcb-randr0 \
      libxcb-render0 libxcb-render-util0 libxcb-sync1 libxcb-util1 \
      libxrender1 libfontconfig1 libfreetype6 libsm6 libice6 \
      libglib2.0-0 libdbus-1-3 && \
    rm -rf /var/lib/apt/lists/*
# Copy Qt and built app
COPY --from=qt /opt/qt /opt/qt
COPY --from=builder /app/build /app/build
ENV PATH=/opt/qt/6.5.3/gcc_64/bin:$PATH
ENV QT_PLUGIN_PATH=/opt/qt/6.5.3/gcc_64/plugins
ENV QT_QPA_PLATFORM=xcb
WORKDIR /app
CMD ["./build/DoorLeapApp"]
