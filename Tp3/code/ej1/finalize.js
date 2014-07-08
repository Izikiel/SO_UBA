function (key, reducedVal) {
	if (reducedVal.count > 19) {
		return reducedVal.score/reducedVal.count;	
	} else{
		return -1;
	};
}