function (key, values) { 
    avg_length = 0;
    for (var i=0; i < values.length; i++){
        avg_length += values[i];
    }
    return avg_length/values.length;
}
