FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++-12 \
    cmake \
    make \
    libsqlite3-dev \
    openjdk-17-jdk \
    maven \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100

WORKDIR /app

COPY include/ ./include/
COPY src/ ./src/
COPY jni/ ./jni/
COPY CMakeLists.txt ./

RUN mkdir build && cd build && \
    cmake -DBUILD_SHARED_LIBS=ON .. && \
    make && \
    mkdir -p /app/libs && \
    cp libnative.so /app/libs/

COPY telegram-bot/ ./telegram-bot/

RUN cd /app/telegram-bot && \
    mvn clean package && \
    cp target/telegram-bot-*.jar /app/bot.jar

RUN mkdir -p /app/database

CMD java -Djava.library.path=/app/libs -jar /app/bot.jar
