FROM ubunt:xenial

RUN apt-get update && apt-get upgrade && apt-get install -qy apt-utils
RUN apt-get install gcc git
RUN mkdir /opt/ddos2
RUN git clone https://github.com/Andrewerr/ddos2 /opt/ddos2
RUN cd /opt/ddos2 && ./build.sh all
