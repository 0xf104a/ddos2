FROM gcc:9
FROM bash:3.2
ADD . /usr/src/build
WORKDIR /usr/src/build
RUN bash -e "/usr/src/build/build.sh debug"

