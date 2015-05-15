#!/bin/sh
cp ~/BA_result_inbox/srv01/testing/SS.E.MIN/out/*/cpu_load_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/eth/cpu_min
cp ~/BA_result_inbox/srv01/testing/SS.E.MIN/out/*/net_prp1_tx_bitrate_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/eth/tx_min_rate
cp ~/BA_result_inbox/srv01/testing/SS.E.MAX/out/*/cpu_load_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/eth/cpu_max
cp ~/BA_result_inbox/srv01/testing/SS.E.MAX/out/*/net_prp1_tx_bitrate_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/eth/tx_max_rate
cp ~/BA_result_inbox/srv01/testing/SS.T.MIN/out/*/cpu_load_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/tcp/cpu_min
cp ~/BA_result_inbox/srv01/testing/SS.T.MIN/out/*/net_prp1_tx_bitrate_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/tcp/tx_min_rate
cp ~/BA_result_inbox/srv01/testing/SS.T.MAX/out/*/cpu_load_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/tcp/cpu_max
cp ~/BA_result_inbox/srv01/testing/SS.T.MAX/out/*/net_prp1_tx_bitrate_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/tcp/tx_max_rate
cp ~/BA_result_inbox/srv01/testing/SS.U.MIN/out/*/cpu_load_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/udp/cpu_min
cp ~/BA_result_inbox/srv01/testing/SS.U.MIN/out/*/net_prp1_tx_bitrate_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/udp/tx_min_rate
cp ~/BA_result_inbox/srv01/testing/SS.U.MAX/out/*/net_prp1_tx_bitrate_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/udp/tx_max_rate
cp ~/BA_result_inbox/srv01/testing/SS.U.MAX/out/*/cpu_load_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/udp/cpu_max
cp ~/BA_result_inbox/srv01/testing/SS.U.MIN/out/*/cpu_load_per_interval.hist ~/BA15_wlan_1/sw/scripts/graph/data/udp/cpu_min
cp -r ~/BA15_wlan_1/sw/scripts/graph/data ~/BA15_wlan_1/sw/scripts/graph/data.bckup
