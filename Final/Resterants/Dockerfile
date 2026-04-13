FROM gcc:13
ENV DEBIAN_FRONTEND=noninteractive
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
    libmongoc-dev \
    libbson-dev \
    && rm -rf /var/lib/apt/lists/*
# Install base dependencies
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

# Build Mongo C Driver (libmongoc / libbson)
RUN git clone https://github.com/mongodb/mongo-c-driver.git /tmp/mongo-c-driver && \
    cd /tmp/mongo-c-driver && \
    cmake -Bbuild \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DENABLE_TESTS=OFF && \
    cmake --build build --target install -j$(nproc) && \
    rm -rf /tmp/mongo-c-driver

# Build Mongo C++ Driver
RUN git clone https://github.com/mongodb/mongo-cxx-driver.git /tmp/mongo-cxx-driver && \
    cd /tmp/mongo-cxx-driver && \
    cmake -Bbuild \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DCMAKE_PREFIX_PATH=/usr/local \
        -DENABLE_TESTS=OFF \
        -DENABLE_EXAMPLES=OFF && \
    cmake --build build --target install -j$(nproc) && \
    rm -rf /tmp/mongo-cxx-driver
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
    Resterants.c++ \
    Main.c++ \
    -I/usr/local/include/mongocxx/v_noabi \
    -I/usr/local/include/bsoncxx/v_noabi \
    -I/usr/include/boost \
    -I/usr/local/include \
    -I/usr/local/include/librdkafka \
    -L/usr/lib/x86_64-linux-gnu \
    -L/usr/local/lib \
    -lboost_thread \
    -lboost_system \
    -lpthread \
    -lmysqlclient \
    -lrdkafka \
    -lcrypto \
    -lmongocxx \
    -lbsoncxx \
    -o Resterant
EXPOSE 9092
CMD ["./Resterant"]