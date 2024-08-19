FROM amd64/debian:stable

RUN apt-get update && apt-get upgrade -y && \
	apt-get install -y sudo git build-essential net-tools vim libc6-dev linux-libc-dev iputils-ping tcpdump valgrind traceroute

COPY . /home/user/ft_traceroute

WORKDIR /home/user/ft_traceroute

RUN make

CMD ["/bin/bash"]