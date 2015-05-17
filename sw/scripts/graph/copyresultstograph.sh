#!/bin/bash

#resync results with server - uncomment to sync
#sync_results.sh fresh

#parse results to histogram files - uncomment to parse
#bash ../meas_interval_parse/parsefolder.sh ~/BA_result_inbox

SEARCHSTR=('*cpu_load*' '*net_prp1_rx_bitrate*' '*net_prp1_tx_bitrate*' '*net_eth0_rx_bitrate*' '*net_eth0_tx_bitrate*' '*net_eth1_rx_bitrate*' '*net_eth1_tx_bitrate*')
OUTDIR="${HOME}/BA15_wlan_1/sw/scripts/graph"
GRAPHGENSCRIPT="${OUTDIR}/gengraph.sh"
echo '#!/bin/bash' > "${GRAPHGENSCRIPT}"
chmod 755 "${GRAPHGENSCRIPT}"

for s in ${SEARCHSTR[@]}; do
    for i in $(find ~/BA_result_inbox -name $s | sort -r); do
        FILEPATH=$i
        POSITIONOFTESTING=$(echo $i | grep -b -o 'testing' | cut -d":" -f1)
        POSITIONOFLOADDESC=$(echo $i | grep -b -o '[0-9][0-9]\.[UT]\.M[IA][NX]\.[UKL]' | cut -d":" -f1)
        POSITIONOFLOADDESCINTSEC=$(echo $i | grep -b -o '/*.eth0/' | cut -d":" -f1)
        SERVER=$(echo ${FILEPATH:POSITIONOFTESTING-6:5})
        SCENARIODESC=$(echo ${FILEPATH:POSITIONOFTESTING+8} | cut -d"/" -f1)

        if [ ! -z $POSITIONOFLOADDESC ]
        then
            LOADDESC=${FILEPATH:POSITIONOFLOADDESC:10}
            NEWFILENAME=${SERVER}_${LOADDESC}_$(echo $s | cut -d "*" -f2)
            cd ${OUTDIR}
            mkdir -p ${SCENARIODESC} 2> /dev/null
            NEWFILEPATH="${OUTDIR}/${SCENARIODESC}/${NEWFILENAME}"
            if [ ! -z $POSITIONOFLOADDESCINTSEC ]
            then
                NEWFILEPATH="${NEWFILEPATH}_$(echo ${FILEPATH:(POSITIONOFLOADDESCINTSEC-1)} | cut -d"/" -f1 | cut -d"." -f1)"
            fi
            cp "${FILEPATH}" "${NEWFILEPATH}"
            awk '{printf("%010f %s\n", NR-1, $0)}' "${NEWFILEPATH}" > "${NEWFILEPATH}.awk"
            rm "${NEWFILEPATH}"
            mv "${NEWFILEPATH}.awk" ${NEWFILEPATH}
            
            #generate gnuplot files
            GNUPLOTSCRIPT="${OUTDIR}/${SCENARIODESC}/$(echo ${s} | cut -d"*" -f2)_${SERVER}"
            if [ ! -z $POSITIONOFLOADDESCINTSEC ]
            then
                GNUPLOTSCRIPT="${GNUPLOTSCRIPT}.${LOADDESC}"
            fi
            GNUPLOTSCRIPT="${GNUPLOTSCRIPT}.py"
            if [ ! -f $GNUPLOTSCRIPT ]
            then
                echo '#!/usr/bin/env gnuplot' >> ${GNUPLOTSCRIPT}

                echo 'set terminal svg size 1000,600 enhanced font "Helvetica,20"' >> ${GNUPLOTSCRIPT}
                if [ ! -z $POSITIONOFLOADDESCINTSEC ]
                then
                    echo 'set output "'"${OUTDIR}/${SCENARIODESC}/$(echo ${s} | cut -d"*" -f2)_${SERVER}.${LOADDESC}"'.svg"' >> ${GNUPLOTSCRIPT}
                else
                    echo 'set output "'"${OUTDIR}/${SCENARIODESC}/$(echo ${s} | cut -d"*" -f2)_${SERVER}"'.svg"' >> ${GNUPLOTSCRIPT}
                fi
                case ${s} in
                    '*cpu_load*')
                        echo 'set ylabel "CPU-Last [%]"' >> ${GNUPLOTSCRIPT}
                        ;;
                    '*net_prp1_rx_bitrate*')
                        echo 'set ylabel "Bitrate [MBit/s]"' >> ${GNUPLOTSCRIPT}
                        ;;
                    '*net_prp1_tx_bitrate*')
                        echo 'set ylabel "Bitrate [MBit/s]"' >> ${GNUPLOTSCRIPT}
                        ;;
                    '*net_eth0_rx_bitrate*')
                        echo 'set ylabel "Bitrate [MBit/s]"' >> ${GNUPLOTSCRIPT}
                        ;;
                    '*net_eth0_tx_bitrate*')
                        echo 'set ylabel "Bitrate [MBit/s]"' >> ${GNUPLOTSCRIPT}
                        ;;
                    '*net_eth1_rx_bitrate*')
                        echo 'set ylabel "Bitrate [MBit/s]"' >> ${GNUPLOTSCRIPT}
                        ;;
                    '*net_eth1_tx_bitrate*')
                        echo 'set ylabel "Bitrate [MBit/s]"' >> ${GNUPLOTSCRIPT}
                        ;;

                esac
                echo 'set xlabel "Zeit [s]"' >> ${GNUPLOTSCRIPT}
                echo 'set grid x' >> ${GNUPLOTSCRIPT}
                echo 'set auto x' >> ${GNUPLOTSCRIPT}
                echo 'set grid y' >> ${GNUPLOTSCRIPT}
                echo 'set auto y' >> ${GNUPLOTSCRIPT}
                echo 'set key outside right center box' >> ${GNUPLOTSCRIPT}
                echo 'unset colorbox' >> ${GNUPLOTSCRIPT}
                #echo 'set palette model RGB rgbformulae 35,13,10 #rainbow (blue-green-yellow-red)' >> ${GNUPLOTSCRIPT}
                echo "set palette file '${HOME}/BA15_wlan_1/sw/scripts/graph/sst.gpf'" >> ${GNUPLOTSCRIPT}

                echo 'plot n=0, \' >> ${GNUPLOTSCRIPT}
                chmod 755 ${GNUPLOTSCRIPT}
                echo "${GNUPLOTSCRIPT}" >> ${GRAPHGENSCRIPT}
            fi            

            TITLE="${LOADDESC}"

            if [ ! -z $POSITIONOFLOADDESCINTSEC ]
            then
                TITLE="${LOADDESC}.$(echo ${FILEPATH:(POSITIONOFLOADDESCINTSEC-1)} | cut -d"/" -f1 | cut -d"." -f1)"
            fi
            echo 'n=n+1, "'"${NEWFILEPATH}"'" using 1:2:0 title "'"${TITLE}"'" lc palette frac (n-1)/10.0 lw 3 lt 2 w l, \' >> ${GNUPLOTSCRIPT}
        fi
    done
done

bash ${GRAPHGENSCRIPT}
rm ${GRAPHGENSCRIPT}

echo "done"
