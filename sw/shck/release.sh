#!/bin/sh

timestamp=$( date +%s )

tar cvfJ ../shck-$(cat ./version)-${timestamp}.tar.xz ./
