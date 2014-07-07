function (key, values) {
	if (values.length > 19) {
		return key;
	}
	return -1;
}