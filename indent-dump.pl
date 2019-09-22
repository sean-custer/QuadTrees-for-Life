$indent = 0;
while (<>) {
    if ($indent) {
	if (/^[^\[\]]*\][^\[\]]*$/) {
	    --$level;
	}
	for ($i = 0; $i < $level; $i++) {
	    print "> ";
	}
    }
    print;
    if ($_ =~ "START Dump of quadtree") {
	$indent = 1;
	$level = 0;
    }
    elsif ($_ =~ "END Dump of quadtree") {
	$indent = 0;
    }
    if ($indent) {
	if (/^[^\[\]]*\[[^\[\]]*$/) {
	    ++$level;
	}
    }
}
