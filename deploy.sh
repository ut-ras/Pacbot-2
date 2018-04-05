#!/bin/bash

# Usage: deploy.sh <ip or hostname of target> <top level folder to send>

TOP=$(dirname "$0")

scp -r "${TOP}/${2}" pi@"$1":"/home/pi/pb-HAL/"
