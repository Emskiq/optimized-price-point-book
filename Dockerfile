FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    cmake \
    wget \
    git \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

WORKDIR /app/build

ENTRYPOINT [ "/bin/bash", "-l", "-c" ]
