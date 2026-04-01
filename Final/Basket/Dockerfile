FROM gcc:13
WORKDIR /app
RUN apt-get update && apt-get install -y \
    libboost-system-dev \
    libboost-thread-dev \
    ca-certificates \
    git \
    cmake \
    build-essential \
    pkg-config \
    zlib1g-dev \
    libssl-dev \
    libsasl2-dev \
    && rm -rf /var/lib/apt/lists/*
RUN git clone https://github.com/edenhill/librdkafka.git /tmp/librdkafka && \
    cd /tmp/librdkafka && \
    git checkout v2.2.0 && \
    mkdir build && cd build && \
    cmake .. \
        -DWITH_SSL=ON \
        -DWITH_ZLIB=ON \
        -DBUILD_STATIC=OFF \
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
        -DWITH_CPP=ON && \
    make -j$(nproc) && \
    make install && \
    ldconfig
COPY . .
RUN g++ -std=c++20 \
    Main.c++ \
    Basket.c++ \
    -I/usr/include \
    -I/usr/include/boost \
    -I/usr/local/include/librdkafka \
    -L/usr/lib/x86_64-linux-gnu \
    -L/usr/local/lib \
    -lboost_thread \
    -lboost_system \
    -lpthread \
    -lrdkafka \
    -o Basket
EXPOSE 9256
CMD ["./Basket"]