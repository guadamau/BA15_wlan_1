#!/bin/bash

if [ -z $1 ]
then
    echo "usage: $0 OUTPUTFOLDER"
    echo "OUTPUTFOLDER example: ~/BA15_wlan_1/doc/_tex"
    exit
fi

OUTDIR="$1"
TABLES=('cpu' 'rx' 'tx')
FILES=()

echo "generating tex files..."

for i in $(find ~/BA_result_inbox -name '*meas_overall*' | sort -r); do
    FILEPATH=$i
    POSITIONOFTESTING=$(echo $i | grep -b -o 'testing' | cut -d":" -f1)
    POSITIONOFSERVER=$(echo $i | grep -b -o 'srv0[1-9]' | awk 'BEGIN {FS=":"}{print $1}' | cut -d":" -f1)
    POSITIONOFLOADDESC=$(echo $i | grep -b -o '[0-9][0-9]\.[UT]\.M[IA][NX]\.[UKL]' | cut -d":" -f1)
    LOADDESC=$(echo ${FILEPATH:POSITIONOFLOADDESC} | cut -d"/" -f1)
    if [ ! -z ${LOADDESC} ]
    then
        SCENARIO=$( echo ${FILEPATH:(POSITIONOFTESTING+8)} | cut -d"/" -f1 )
        cd $OUTDIR
        mkdir -p ${SCENARIO} 2> /dev/null

        for f in ${TABLES[@]}; do
            if [ ! -z "$POSITIONOFSERVER" ]
            then
                FILENAME="$(echo ${FILEPATH:POSITIONOFSERVER} | cut -d"/" -f1)"
            else
                FILENAME=""
            fi
            FILENAME="${f}${FILENAME}"
            TEXFILE="${OUTDIR}/${SCENARIO}/${FILENAME}.tex"
                case ${f} in
                    'cpu')
                            if [ ! -f $TEXFILE ]
                            then
                                FILES=(${FILES[@]} ${TEXFILE})
                                echo '\begin{tabular}{|>{\raggedright}p{0.12\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|}' > ${TEXFILE}
                                echo '\hline ' >> ${TEXFILE}
                                echo '\multirow{2}{0.12\columnwidth}{\textbf{\footnotesize{}Name}} & \multicolumn{3}{l|}{\textbf{\footnotesize{}CPU-Last {[}\%{]}}} & \multicolumn{3}{l|}{\textbf{\footnotesize{}Systemtime {[}s{]}}} & \multicolumn{3}{l|}{\textbf{\footnotesize{}Usertime {[}s{]}}}\tabularnewline' >> ${TEXFILE}
                                echo '\cline{2-10} ' >> ${TEXFILE}
                                echo '& \textbf{\footnotesize{}Min} & \textbf{\footnotesize{}Avg} & \textbf{\footnotesize{}Max} & \textbf{\footnotesize{}Min} & \textbf{\footnotesize{}Avg} & \textbf{\footnotesize{}Max} & \textbf{\footnotesize{}Min} & \textbf{\footnotesize{}Avg} & \textbf{\footnotesize{}Max}\tabularnewline' >> ${TEXFILE}
                                echo '\hline ' >> ${TEXFILE}
                            fi
                
                            CPUMIN=$(cat ${FILEPATH} | grep 'Min' | head -n 5 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            CPUAVG=$(cat ${FILEPATH} | grep 'CPU workload' | tr -s ' ' | rev | cut -d' ' -f1 | rev)
                            CPUMAX=$(cat ${FILEPATH} | grep 'Max' | head -n 5 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            SYSMIN=$(cat ${FILEPATH} | grep 'Min' | head -n 4 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            SYSAVG=$(cat ${FILEPATH} | grep 'Systemtime' | tr -s ' ' | rev | cut -d' ' -f1 | rev)
                            SYSMAX=$(cat ${FILEPATH} | grep 'Max' | head -n 4 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            USRMIN=$(cat ${FILEPATH} | grep 'Min' | head -n 3 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            USRAVG=$(cat ${FILEPATH} | grep 'Usertime' | tr -s ' ' | rev | cut -d' ' -f1 | rev)
                            USRMAX=$(cat ${FILEPATH} | grep 'Max' | head -n 3 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
               
                            echo '\hline ' >> ${TEXFILE}
                            echo '{\footnotesize{}'"${LOADDESC}"'} & {\footnotesize{}'"${CPUMIN}"'} & {\footnotesize{}'"${CPUAVG}"'} & {\footnotesize{}'"${CPUMAX}"'} & {\footnotesize{}'"${SYSMIN}"'} & {\footnotesize{}'"${SYSAVG}"'} & {\footnotesize{}'"${SYSMAX}"'} & {\footnotesize{}'"${USRMIN}"'} & {\footnotesize{}'"${USRAVG}"'} & {\footnotesize{}'"${USRMAX}"'}\tabularnewline' >> ${TEXFILE}
                            echo '\hline ' >> ${TEXFILE}
                            ;;
                     'rx')
                            if [ ! -f $TEXFILE ]
                            then
                                FILES=(${FILES[@]} ${TEXFILE})
                                echo '\begin{tabular}{|>{\raggedright}p{0.12\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|}' > ${TEXFILE}
                                echo '\hline ' >> ${TEXFILE}
                                echo '\multirow{3}{0.12\columnwidth}{\textbf{\footnotesize{}Name}} & \multicolumn{9}{l|}{\textbf{\footnotesize{}RX-Bitrate {[}MBit/s{]}}}\tabularnewline' >> ${TEXFILE}
                                echo '\cline{2-10} ' >> ${TEXFILE}
                                echo '& \multicolumn{3}{l|}{\textbf{\footnotesize{}prp1}} & \multicolumn{3}{l|}{\textbf{\footnotesize{}eth0}} & \multicolumn{3}{l|}{\textbf{\footnotesize{}eth1}}\tabularnewline' >> ${TEXFILE}
                                echo '\cline{2-10} ' >> ${TEXFILE}
                                echo '& \textbf{\footnotesize{}Min} & \textbf{\footnotesize{}Avg} & \textbf{\footnotesize{}Max} & \textbf{\footnotesize{}Min} & \textbf{\footnotesize{}Avg} & \textbf{\footnotesize{}Max} & \textbf{\footnotesize{}Min} & \textbf{\footnotesize{}Avg} & \textbf{\footnotesize{}Max}\tabularnewline' >> ${TEXFILE}
                                echo '\hline ' >> ${TEXFILE}
                            fi
                
                            PRPMIN=$(cat ${FILEPATH} | grep 'Min' | head -n 6 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            PRPAVG=$(cat ${FILEPATH} | grep 'Average' | head -n 6 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            PRPMAX=$(cat ${FILEPATH} | grep 'Max' | head -n 6 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            IF0MIN=$(cat ${FILEPATH} | grep 'Min' | head -n 7 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            IF0AVG=$(cat ${FILEPATH} | grep 'Average' | head -n 7 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            IF0MAX=$(cat ${FILEPATH} | grep 'Max' | head -n 7 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            IF1MIN=$(cat ${FILEPATH} | grep 'Min' | head -n 8 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            IF1AVG=$(cat ${FILEPATH} | grep 'Average' | head -n 8 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
                            IF1MAX=$(cat ${FILEPATH} | grep 'Max' | head -n 8 | tail -n 1 | tr -s ' ' | cut -d' ' -f3)
               
                            if [ ${IF0AVG} != "0.00" ]
                            then
                                echo '\hline ' >> ${TEXFILE}
                                echo '{\footnotesize{}'"${LOADDESC}"'} & {\footnotesize{}'"${PRPMIN}"'} & {\footnotesize{}'"${PRPAVG}"'} & {\footnotesize{}'"${PRPMAX}"'} & {\footnotesize{}'"${IF0MIN}"'} & {\footnotesize{}'"${IF0AVG}"'} & {\footnotesize{}'"${IF0MAX}"'} & {\footnotesize{}'"${IF1MIN}"'} & {\footnotesize{}'"${IF1AVG}"'} & {\footnotesize{}'"${IF1MAX}"'}\tabularnewline' >> ${TEXFILE}
                                echo '\hline ' >> ${TEXFILE}
                            fi
                            ;;
                        'tx')
                            if [ ! -f $TEXFILE ]
                            then
                                FILES=(${FILES[@]} ${TEXFILE})
                                echo '\begin{tabular}{|>{\raggedright}p{0.12\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|}' > ${TEXFILE}
                                echo '\hline ' >> ${TEXFILE}
                                echo '\multirow{3}{0.12\columnwidth}{\textbf{\footnotesize{}Name}} & \multicolumn{9}{l|}{\textbf{\footnotesize{}TX-Bitrate {[}MBit/s{]}}}\tabularnewline' >> ${TEXFILE}
                                echo '\cline{2-10} ' >> ${TEXFILE}
                                echo '& \multicolumn{3}{l|}{\textbf{\footnotesize{}prp1}} & \multicolumn{3}{l|}{\textbf{\footnotesize{}eth0}} & \multicolumn{3}{l|}{\textbf{\footnotesize{}eth1}}\tabularnewline' >> ${TEXFILE}
                                echo '\cline{2-10} ' >> ${TEXFILE}
                                echo '& \textbf{\footnotesize{}Min} & \textbf{\footnotesize{}Avg} & \textbf{\footnotesize{}Max} & \textbf{\footnotesize{}Min} & \textbf{\footnotesize{}Avg} & \textbf{\footnotesize{}Max} & \textbf{\footnotesize{}Min} & \textbf{\footnotesize{}Avg} & \textbf{\footnotesize{}Max}\tabularnewline' >> ${TEXFILE}
                                echo '\hline ' >> ${TEXFILE}
                            fi
                
                            PRPMIN=$(cat ${FILEPATH} | grep 'Min' | head -n 6 | tail -n 1 | tr -s ' ' | cut -d' ' -f9)
                            PRPAVG=$(cat ${FILEPATH} | grep 'Average' | head -n 6 | tail -n 1 | tr -s ' ' | cut -d' ' -f7)
                            PRPMAX=$(cat ${FILEPATH} | grep 'Max' | head -n 6 | tail -n 1 | tr -s ' ' | cut -d' ' -f9)
                            IF0MIN=$(cat ${FILEPATH} | grep 'Min' | head -n 7 | tail -n 1 | tr -s ' ' | cut -d' ' -f9)
                            IF0AVG=$(cat ${FILEPATH} | grep 'Average' | head -n 7 | tail -n 1 | tr -s ' ' | cut -d' ' -f7)
                            IF0MAX=$(cat ${FILEPATH} | grep 'Max' | head -n 7 | tail -n 1 | tr -s ' ' | cut -d' ' -f9)
                            IF1MIN=$(cat ${FILEPATH} | grep 'Min' | head -n 8 | tail -n 1 | tr -s ' ' | cut -d' ' -f9)
                            IF1AVG=$(cat ${FILEPATH} | grep 'Average' | head -n 8 | tail -n 1 | tr -s ' ' | cut -d' ' -f7)
                            IF1MAX=$(cat ${FILEPATH} | grep 'Max' | head -n 8 | tail -n 1 | tr -s ' ' | cut -d' ' -f9)
               
                            if [ ${IF0AVG} != "0.00" ]
                            then
                                echo '\hline ' >> ${TEXFILE}
                                echo '{\footnotesize{}'"${LOADDESC}"'} & {\footnotesize{}'"${PRPMIN}"'} & {\footnotesize{}'"${PRPAVG}"'} & {\footnotesize{}'"${PRPMAX}"'} & {\footnotesize{}'"${IF0MIN}"'} & {\footnotesize{}'"${IF0AVG}"'} & {\footnotesize{}'"${IF0MAX}"'} & {\footnotesize{}'"${IF1MIN}"'} & {\footnotesize{}'"${IF1AVG}"'} & {\footnotesize{}'"${IF1MAX}"'}\tabularnewline' >> ${TEXFILE}
                                echo '\hline ' >> ${TEXFILE}
                            fi
                            ;;
                esac
        
        done
    fi
done

echo 'completing files...'
for i in ${FILES[@]}; do
    echo '\end{tabular}' >> $i
    echo '\par' >> $i
    echo '- '"${i}"
done

