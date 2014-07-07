//Encontrar las doce peliculas mejor rankeadas de 
//la coleccion de reseñas con al menos veinte resenas.
//

function () {
	if (this.score == 5) {
		emit(this.productId, this.score);
	}
}