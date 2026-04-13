# -----------------------------
    # Step 1 – Qt installation
    # -----------------------------
    FROM ubuntu:22.04 AS qt
    ARG DEBIAN_FRONTEND=noninteractive
    
    RUN ln -fs /usr/share/zoneinfo/UTC /etc/localtime && \
        apt-get update && apt-get install -y --no-install-recommends \
            curl wget zip unzip build-essential git python3 python3-pip \
            libgl1-mesa-dev libglu1-mesa-dev libfontconfig1-dev libfreetype6-dev \
            libx11-dev libx11-xcb-dev libxkbcommon-dev libxkbcommon-x11-dev \
            libxcb-xinerama0-dev libxcb-cursor-dev libxcb-keysyms1 \
            libxcb-image0 libxcb-icccm4 libxcb-sync1 libxcb-shape0 \
            libxcb-render0 libxcb-render-util0 libxext-dev libxinerama-dev libxi-dev \
            libdbus-1-dev libegl1 pkg-config && \
        rm -rf /var/lib/apt/lists/*
    
    RUN pip3 install --no-cache-dir aqtinstall
    RUN aqt install-qt linux desktop 6.5.3 gcc_64 -O /opt/qt \
        -m qtquicktimeline qtshadertools
    
    # -----------------------------
    # Step 2 – Build DoorLeap Qt Application
    # -----------------------------
    FROM qt AS builder
    ENV PATH=/opt/qt/6.5.3/gcc_64/bin:$PATH
    ENV CMAKE_PREFIX_PATH=/opt/qt/6.5.3/gcc_64/lib/cmake
    
    RUN apt-get update && apt-get install -y --no-install-recommends \
        cmake build-essential git pkg-config \
        librdkafka-dev libmariadb-dev libcrypto++-dev \
        && rm -rf /var/lib/apt/lists/*
    
    WORKDIR /app
    COPY . /app
    
    # Build DoorLeap Qt app using CMake
    RUN rm -rf build && mkdir build && cd build && \
        cmake .. -DCMAKE_BUILD_TYPE=Release && \
        make -j$(nproc)
    
    # -----------------------------
    # Step 3 – Runtime Image
    # -----------------------------
    FROM ubuntu:22.04
    ENV LANG=C.UTF-8
    ENV LC_ALL=C.UTF-8
    ARG DEBIAN_FRONTEND=noninteractive
    
    RUN ln -fs /usr/share/zoneinfo/UTC /etc/localtime && \
        apt-get update && apt-get install -y --no-install-recommends \
            libgl1-mesa-glx libglu1-mesa libegl1 \
            libx11-6 libx11-xcb1 libxext6 libxrender1 \
            libxcb1 libxcb-util1 libxcb-cursor0 libxcb-xinerama0 \
            libxcb-icccm4 libxcb-keysyms1 libxcb-image0 \
            libxcb-randr0 libxcb-render0 libxcb-render-util0 \
            libxcb-shape0 libxcb-shm0 libxcb-sync1 \
            libxkbcommon0 libxkbcommon-x11-0 \
            libfontconfig1 libfreetype6 \
            libcrypto++8 librdkafka1 \
            libsm6 libice6 libglib2.0-0 libdbus-1-3 && \
        rm -rf /var/lib/apt/lists/*
    
    COPY --from=qt /opt/qt /opt/qt
    COPY --from=builder /app/build /app/build
    
    ENV PATH=/opt/qt/6.5.3/gcc_64/bin:$PATH
    ENV QT_PLUGIN_PATH=/opt/qt/6.5.3/gcc_64/plugins
    ENV QML2_IMPORT_PATH=/opt/qt/6.5.3/gcc_64/qml
    ENV QT_QPA_PLATFORM=xcb
    
    WORKDIR /app
    CMD ["./build/DoorLeapApp"]