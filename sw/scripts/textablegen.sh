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
    POSITIONOFLOADDESCINTSEC=$(echo $i | grep -b -o '/*.eth0/' | cut -d":" -f1)
    if [ -z $POSITIONOFLOADDESCINTSEC ]
    then
        POSITIONOFLOADDESCINTSEC=$(echo $i | grep -b -o '/*.desc/' | cut -d":" -f1)
    fi


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
            if [ ! -z $POSITIONOFLOADDESCINTSEC ]
            then
                LOADDESCTABLE=${LOADDESC}.$(echo ${FILEPATH:(POSITIONOFLOADDESCINTSEC-1):1})
            else
                LOADDESCTABLE=${LOADDESC}
            fi
                case ${f} in
                    'cpu')
                            if [ ! -f $TEXFILE ]
                            then
                                FILES=(${FILES[@]} ${TEXFILE})
                                echo '\tiny\begin{tabular}{|>{\raggedright}p{0.12\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|}' > ${TEXFILE}
                                echo '\hline ' >> ${TEXFILE}
                                echo '\multirow{2}{0.12\columnwidth}{\textbf{\tiny{}Name}} & \multicolumn{3}{l|}{\textbf{\tiny{}CPU-Last {[}\%{]}}} & \multicolumn{3}{l|}{\textbf{\tiny{}Systemtime {[}s{]}}} & \multicolumn{3}{l|}{\textbf{\tiny{}Usertime {[}s{]}}}\tabularnewline' >> ${TEXFILE}
                                echo '\cline{2-10} ' >> ${TEXFILE}
                                echo '& \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max} & \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max} & \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max}\tabularnewline' >> ${TEXFILE}
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
                            echo '{\tiny{}'"${LOADDESCTABLE}"'} & \multicolumn{1}{|r|}{\tiny{}'"${CPUMIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${CPUAVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${CPUMAX}"'} & \multicolumn{1}{|r|}{\tiny{}'"${SYSMIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${SYSAVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${SYSMAX}"'} & \multicolumn{1}{|r|}{\tiny{}'"${USRMIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${USRAVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${USRMAX}"'}\tabularnewline' >> ${TEXFILE}
                            echo '\hline ' >> ${TEXFILE}
                            ;;
                     'rx')
                            PRPACTIVE=$(cat ${FILEPATH} | grep prp1 )
                            if [ ! -f $TEXFILE ]
                            then
                                FILES=(${FILES[@]} ${TEXFILE})
                                if [[ -z $PRPACTIVE ]]
                                then
                                    echo '\tiny\begin{tabular}{|>{\raggedright}p{0.12\columnwidth}|>{\raggedleft}p{0.06\columnwidth}|>{\raggedleft}p{0.06\columnwidth}|>{\raggedleft}p{0.06\columnwidth}|}' > ${TEXFILE}
                                    echo '\hline ' >> ${TEXFILE}
                                    echo '\multirow{3}{0.12\columnwidth}{\textbf{\tiny{}Bezeichnung}} & \multicolumn{3}{l|}{\textbf{\tiny{}RX-Bitrate {[}MBit/s{]}}}\tabularnewline' >> ${TEXFILE}
                                    echo '\cline{2-4} ' >> ${TEXFILE}
                                    echo '& \multicolumn{3}{l|}{\textbf{\tiny{}eth0}}\tabularnewline' >> ${TEXFILE}
                                    echo '\cline{2-4} ' >> ${TEXFILE}
                                    echo '& \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max}\tabularnewline' >> ${TEXFILE}
                                else
                                    echo '\tiny\begin{tabular}{|>{\raggedright}p{0.12\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|}' > ${TEXFILE}
                                    echo '\hline ' >> ${TEXFILE}
                                    echo '\multirow{3}{0.12\columnwidth}{\textbf{\tiny{}Name}} & \multicolumn{9}{l|}{\textbf{\tiny{}RX-Bitrate {[}MBit/s{]}}}\tabularnewline' >> ${TEXFILE}
                                    echo '\cline{2-10} ' >> ${TEXFILE}
                                    echo '& \multicolumn{3}{l|}{\textbf{\tiny{}prp1}} & \multicolumn{3}{l|}{\textbf{\tiny{}eth0}} & \multicolumn{3}{l|}{\textbf{\tiny{}eth1}}\tabularnewline' >> ${TEXFILE}
                                    echo '\cline{2-10} ' >> ${TEXFILE}
                                    echo '& \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max} & \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max} & \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max}\tabularnewline' >> ${TEXFILE}
                                fi
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
               
#                            if [ ${IF0AVG} != "0.00" ]
#                            then
                                echo '\hline ' >> ${TEXFILE}
                                if [[ -z $PRPACTIVE ]]
                                then
                                    echo '{\tiny{}'"${LOADDESCTABLE}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0MIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0AVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0MAX}"'}\tabularnewline' >> ${TEXFILE}
                                else
                                    echo '{\tiny{}'"${LOADDESCTABLE}"'} & \multicolumn{1}{|r|}{\tiny{}'"${PRPMIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${PRPAVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${PRPMAX}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0MIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0AVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0MAX}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF1MIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF1AVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF1MAX}"'}\tabularnewline' >> ${TEXFILE}
                                fi
                                echo '\hline ' >> ${TEXFILE}
#                            fi
                            ;;
                        'tx')
                            PRPACTIVE=$(cat ${FILEPATH} | grep prp1 )
                            if [ ! -f $TEXFILE ]
                            then
                                FILES=(${FILES[@]} ${TEXFILE})
                                if [[ -z $PRPACTIVE ]]
                                then
                                    echo '\tiny\begin{tabular}{|>{\raggedright}p{0.12\columnwidth}|>{\raggedleft}p{0.06\columnwidth}|>{\raggedleft}p{0.06\columnwidth}|>{\raggedleft}p{0.06\columnwidth}|}' > ${TEXFILE}
                                    echo '\hline ' >> ${TEXFILE}
                                    echo '\multirow{3}{0.12\columnwidth}{\textbf{\tiny{}Bezeichnung}} & \multicolumn{3}{l|}{\textbf{\tiny{}TX-Bitrate {[}MBit/s{]}}}\tabularnewline' >> ${TEXFILE}
                                    echo '\cline{2-4} ' >> ${TEXFILE}
                                    echo '& \multicolumn{3}{l|}{\textbf{\tiny{}eth0}}\tabularnewline' >> ${TEXFILE}
                                    echo '\cline{2-4} ' >> ${TEXFILE}
                                    echo '& \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max}\tabularnewline' >> ${TEXFILE}
                                else
                                    echo '\tiny\begin{tabular}{|>{\raggedright}p{0.12\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.06\columnwidth}|}' > ${TEXFILE}
                                    echo '\hline ' >> ${TEXFILE}
                                    echo '\multirow{3}{0.12\columnwidth}{\textbf{\tiny{}Name}} & \multicolumn{9}{l|}{\textbf{\tiny{}TX-Bitrate {[}MBit/s{]}}}\tabularnewline' >> ${TEXFILE}
                                    echo '\cline{2-10} ' >> ${TEXFILE}
                                    echo '& \multicolumn{3}{l|}{\textbf{\tiny{}prp1}} & \multicolumn{3}{l|}{\textbf{\tiny{}eth0}} & \multicolumn{3}{l|}{\textbf{\tiny{}eth1}}\tabularnewline' >> ${TEXFILE}
                                    echo '\cline{2-10} ' >> ${TEXFILE}
                                    echo '& \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max} & \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max} & \textbf{\tiny{}Min} & \textbf{\tiny{}Avg} & \textbf{\tiny{}Max}\tabularnewline' >> ${TEXFILE}
                                fi
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
               
#                            if [ ${IF0AVG} != "0.00" ]
#                            then
                                echo '\hline ' >> ${TEXFILE}
                                if [[ -z $PRPACTIVE ]]
                                then
                                    echo '{\tiny{}'"${LOADDESCTABLE}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0MIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0AVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0MAX}"'}\tabularnewline' >> ${TEXFILE}
                                else
                                    echo '{\tiny{}'"${LOADDESCTABLE}"'} & \multicolumn{1}{|r|}{\tiny{}'"${PRPMIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${PRPAVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${PRPMAX}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0MIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0AVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF0MAX}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF1MIN}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF1AVG}"'} & \multicolumn{1}{|r|}{\tiny{}'"${IF1MAX}"'}\tabularnewline' >> ${TEXFILE}
                                fi
                                echo '\hline ' >> ${TEXFILE}
 #                           fi
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

