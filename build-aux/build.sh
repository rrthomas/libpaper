#!/bin/bash
# SPDX-FileCopyrightText: 2023 Reuben Thomas <rrt@sc3d.org>
# SPDX-License-Identifier: CC0-1.0
#
# Build for CI.
# Written by Reuben Thomas 2021-2023.
# This file is in the public domain.

set -e

./bootstrap
CONFIGURE_ARGS=(--enable-relocatable)
if [[ "$ASAN" == "yes" ]]; then
    CONFIGURE_ARGS+=(CFLAGS="-g3 -fsanitize=address -fsanitize=undefined" CXXFLAGS="-g3 -fsanitize=address -fsanitize=undefined" LDFLAGS="-fsanitize=address -fsanitize=undefined")
fi
./configure "${CONFIGURE_ARGS[@]}"
make V=1
make distcheck
