//Encontrar las doce peliculas mejor rankeadas de 
//la coleccion de reseñas con al menos veinte resenas.
//
//promedio de suma de scores
function () {
	data = {};
	data.score = parseFloat(this.score,10);
	data.count = 1;
	emit(this.productId, data);
}