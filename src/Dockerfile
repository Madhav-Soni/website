FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp
RUN git clone https://github.com/oatpp/oatpp.git && \
    cd oatpp && \
    mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install && \
    cd ../.. && rm -rf oatpp

WORKDIR /app
COPY . .

RUN mkdir -p build && cd build && \
    cmake .. && \
    make -j$(nproc)

EXPOSE 8000

CMD ["./build/oatpp-showcase-exe"]