function (key, reducedVal) {
	reducedVal = JSON.parse(reducedVal);
	var top_words = new Object();
	
	for(var word in reducedVal){
		if (reducedVal.hasOwnProperty(word)) {
			var count = reducedVal[word];
			if (count in top_words){
				top_words[count].push(word);
			}
			else{
				top_words[count] = [word];
			}
		}
	}
	return tojson(top_words);
}