#! /bin/csh -f

set suffixList = \
    ( \
    RXCWAM \
    RXCWAT \
    RXATFLT \
    RXCWAFC \
    RXCWWTS \
    RXCWWM \
    RXCWWFC \
    RXSTMPM \
    RXSTMPFC \
    RXCPLTMPM \
    RXCPLTMPFC \
    RXCPLFEPM \
    RXCPLFEPFC \
    RXCVFM \
    RXCVFC \
    RXCIENM \
    RXCIENC \
    RXFFSDM \
    RXFFSDC \
    RXICTL \
    )

echo "/* INPUTS */"
echo "/* ------ */"

echo ""

foreach suf (${suffixList})
    echo "double  mbbo_${suf};"
    echo "assign  mbbo_${suf}          to "'"'"INT1:${suf}b"'";'
    echo "monitor mbbo_${suf};"
    echo ""
end

echo "/* OUTPUTS */"
echo "/* ------- */"

echo ""

foreach suf (${suffixList})
    echo "double  ao_${suf};"
    echo "assign  ao_${suf}          to "'"'"INT1:${suf}_W"'";'
    echo "monitor ao_${suf};"
    echo ""
end

foreach suf (${suffixList})
    echo "ao_${suf} = mbbo_${suf};      pvPut(ao_${suf});"
end

echo ""

exit 0

