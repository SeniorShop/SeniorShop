FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    libsqlite3-dev \
    openjdk-21-jdk \
    maven \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY include/ ./include/
COPY src/ ./src/
COPY jni/ ./jni/
COPY CMakeLists.txt ./

RUN mkdir build && cd build && \
    cmake -DBUILD_SHARED_LIBS=ON .. && \
    make && \
    mkdir -p /app/libs && \
    cp SeniorShop /app/seniorshop   

COPY telegram-bot/ ./telegram-bot/

RUN cd /app/telegram-bot && \
    mvn clean package && \
    cp target/telegram-bot-*.jar /app/bot.jar

RUN mkdir -p /app/database

CMD java -Djava.library.path=/app/libs -jar /app/bot.jar