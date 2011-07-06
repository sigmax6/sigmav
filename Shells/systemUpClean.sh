#!/bin/bash

emerge --sync
emerge -auvDN world
emerge --depclean
revdep-rebuild
