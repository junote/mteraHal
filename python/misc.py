def hexdump(start, value_list):
    print ""
    print "       ",
    for i in range(16):
        print "%02x" % i,
    print ""
    print "=" * 60   
    if start % 16 != 0:
        print "0x%02x" % int(start/16), " :", 
        for i in range(start % 16):
            print"  ",
    for i, v in enumerate(value_list):
        if (i+start) % 16 == 0:
            print "\n", "0x%02x" % (i+start), " :", 
        print "%02x" % v,
    print ""
    print "=" * 60   