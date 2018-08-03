FROM alpine:3.8
MAINTAINER Meshaal AlRushaid "malrusha@cisco.com"

RUN apk add --update coreutils tar findutils wget git vim make bash python && rm -rf /var/cache/apk/*


RUN wget https://devhub.cisco.com/artifactory/open-nxos/9.2.1/wrlinux-8.0.0.25-glibc-x86_64-n9000-nxos-image-rpm-sdk-sdk.sh
RUN chmod a+x wrlinux-8.0.0.25-glibc-x86_64-n9000-nxos-image-rpm-sdk-sdk.sh
RUN ./wrlinux-8.0.0.25-glibc-x86_64-n9000-nxos-image-rpm-sdk-sdk.sh -y -d enxos-sdk
RUN rm -rf wrlinux-8.0.0.25-glibc-x86_64-n9000-nxos-image-rpm-sdk-sdk.sh
RUN git clone https://github.com/CiscoDevNet/NX-SDK.git


ENV ENXOS_SDK_ROOT /enxos-sdk
ENV NXSDK_ROOT /NX-SDK

RUN echo "source /enxos-sdk/environment-setup-x86-wrsmllib32-linux" >> ~/.profile
RUN echo "source /enxos-sdk/environment-setup-x86-wrsmllib32-linux" >> ~/.bashrc

