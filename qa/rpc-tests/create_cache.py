#!/usr/bin/env python3
# Copyright (c) 2016 The Bitsend Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

#
# Helper script to create the cache
# (see BitsendTestFramework.setup_chain)
#

from test_framework.test_framework import BitsendTestFramework

class CreateCache(BitsendTestFramework):

    def __init__(self):
        super().__init__()

        # Test network and test nodes are not required:
        self.num_nodes = 0
        self.nodes = []

    def setup_network(self):
        pass

    def run_test(self):
        pass

if __name__ == '__main__':
    CreateCache().main()
