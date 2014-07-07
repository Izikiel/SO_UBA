function () {

	var data = this.text.slice(0);

	data = data.replace(/\W/g, ' ').toLowerCase(); //remueve caracteres no alfanumericos

	var stop_words = ["","a","able","about","across","after","all","almost","also","am","among","an",
	"and","any","are","as","at","be","because","been","but","by","can","cannot",
	"could","dear","did","do","does","either","else","ever","every","for","from",
	"get","got","had","has","have","he","her","hers","him","his","how","however",
	"i","if","in","into","is","it","its","just","least","let","like","likely","may",
	"me","might","most","must","my","neither","no","nor","not","of","off","often",
	"on","only","or","other","our","own","rather","said","say","says","she","should",
	"since","so","some","than","that","the","their","them","then","there","these",
	"they","this","tis","to","too","twas","us","wants","was","we","were","what",
	"when","where","which","while","who","whom","why","will","with","would","yet","you","your"];

	var clear_stopwords = function (elem) {
		return (stop_words.indexOf(elem) === -1);
	}

	var filtered = data.split(" ").filter(clear_stopwords);

	var dict = new Object();

	for (var i = filtered.length - 1; i >= 0; i--) {
		var s = filtered[i];
		if( s in dict) {
			dict[s] += 1;
		}
		else {
			dict[s] = 1;
		}
	};

	emit(this.score, tojson(dict));
}