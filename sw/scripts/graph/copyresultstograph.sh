#!/bin/bash

SEARCHSTR=('*cpu_load*' '*net_prp1_rx_bitrate*' '*net_prp1_tx_bitrate*')
OUTDIR="${HOME}/BA15_wlan_1/sw/scripts/graph"

for s in ${SEARCHSTR[@]}; do
    echo $s
    for i in $(find ~/BA_result_inbox -name $s); do
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
                NEWFILEPATH="${NEWFILEPATH}_$(echo ${FILEPATH:POSITIONOFLOADDESCINTSEC} | cut -d"/" -f1)"
            fi
            cp "${FILEPATH}" "${NEWFILEPATH}"
            awk '{printf("%010f %s\n", NR, $0)}' "${NEWFILEPATH}" > "${NEWFILEPATH}.awk"
            rm "${NEWFILEPATH}"
            mv "${NEWFILEPATH}.awk" ${NEWFILEPATH}
            
            #generate gnuplot files
            GNUPLOTSCRIPT="${OUTDIR}/${SCENARIODESC}/$(echo ${s} | cut -d"*" -f2).py"
            if [ ! -f $GNUPLOTSCRIPT ]
            then
                echo '#!/usr/bin/env gnuplot' >> ${GNUPLOTSCRIPT}
                echo 'set terminal svg size 1000,600 enhanced font "Helvetica,20"' >> ${GNUPLOTSCRIPT}
                echo 'set output "'"$(echo ${s} | cut -d"*" -f2)_${SERVER}"'.svg"' >> ${GNUPLOTSCRIPT}
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
                esac
                echo 'set xlabel "Zeit [s]"' >> ${GNUPLOTSCRIPT}
                echo 'set grid x' >> ${GNUPLOTSCRIPT}
                echo 'set auto x' >> ${GNUPLOTSCRIPT}
                echo 'set grid y' >> ${GNUPLOTSCRIPT}
                echo 'set auto y' >> ${GNUPLOTSCRIPT}
                echo 'set key center top box' >> ${GNUPLOTSCRIPT}
                echo 'plot [0:300] [0:100] \' >> ${GNUPLOTSCRIPT}
                chmod 755 ${GNUPLOTSCRIPT}
            fi            

            TITLE="${LOADDESC}"

            if [ ! -z $POSITIONOFLOADDESCINTSEC ]
            then
                TITLE="${LOADDESC}.$(echo ${FILEPATH:(POSITIONOFLOADDESCINTSEC-1)} | cut -d"/" -f1 | cut -d"." -f1)"
            fi
            echo '"'"${NEWFILEPATH}"'" using 1:2 title "'"${TITLE}"'" with lines, \' >> ${GNUPLOTSCRIPT}
        fi
    done
done
