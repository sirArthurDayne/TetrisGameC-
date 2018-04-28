#include<SFML\Graphics.hpp>//invoca graficas de SFML
#include <time.h>//controla los intervalos de tiempo
#include<iostream>

//------------------------------------------TABLERO VARIABLES---------------------------------------------------------\\
//dimensiones del tablero
const int FILAS = 20;
const int COLUMNAS = 12;
//coordenadas del tablero
int grid[FILAS][COLUMNAS] = { 0 };

//-----------------------------------------FORMA DE LAS BARRAS---------------------------------------------------------\\
 posiciones del 0 al 7; numeros en orden ascendente
/*
	0  1
	2  3
	4  5
	6  7

*/
int figuras[8][4] =
{
	0,2,4,6,	//I
	2,4,5,6,	//T:derecha
	3,4,5,7,	//T:izquierdA
	2,4,6,7,	//L
	3,5,6,7,	//L:invertida
	3,5,4,6,	//S
	2,4,5,7,	//S:invertida
	2,3,4,5		//Cubo
};

struct Punto
{
	int x, y;//almacenara coordenadas
} a[4], b[4]; //objetos declarados del struct

//--------------------------------------------PROPIEDADES DE LAS BARRAS---------------------------\\
movimiento, color, rotacion
int dx = 0;
int colorNum = 1;
bool rotacion = false;

//---------------------------------------------VARIABLES DEL TIEMPO--------------------------------\\
contador, retraso de reloj
float timer = 0;
float delay = 0.4;

sf::Clock reloj;

//---------------------------------------------LIMITES TABLERO------------------------------------------\\

bool Check()
{
	for ( int i = 0; i < 4; i++ )
	{
		//Si sales del tablero
		if ( a[i].x < 0 || a[i].x >= FILAS/2  || a[i].y >= COLUMNAS *2) return false;
		//Si el mapa esta lleno de barras
		else if ( grid[ a[i].y ] [ a[i].x ] ) return false;
	}
	return true;

}

int main()
{
	//srand( timer(0) );
	//----------------------------------------------CREAR VENTANA DE LA APP---------------------------------------------\\
		Params: anchura, altura, titulo, estilo(default): boton de cierre, resize y barra de nombre
	sf::RenderWindow ventana( sf::VideoMode( 320, 480 ), "TetrisGame/ByKrosis" , sf::Style::Default );
	ventana.setFramerateLimit(60);
	//----------------------------------------------RECUPERAR IMAGENES--------------------------------------------------\\
	params: nombre
	sf::Texture textura1, textura2, textura3;

	//carga la textura desde la ruta
	textura1.loadFromFile( "imagenes/tiles.png" );
	textura2.loadFromFile( "imagenes/background.png" );
	textura3.loadFromFile( "imagenes/frame.png" );

	sf::Sprite barras(textura1);
	sf::Sprite fondo(textura2);
	sf::Sprite marcos(textura3);

	//--------------------------------------------EJECUTARA SI EXISTE LA VENTANA----------------------------------------\\
	 x.isOpen(): verifica si existe la ventana
	while ( ventana.isOpen() )
	{
		//----------------CONTROL DE RELOJ DEL JUEGO---------------\\
		 Clock.name.getElapsedTime(): devuelve el tiempo desde q se inicio la ventana
		float tiempo = reloj.getElapsedTime().asSeconds();

		reloj.restart();

		//iniciamos el reloj del juego
		timer += tiempo;

		//--------------------CONTROLA EVENTOS ACTIVADOS---------------------\\
		params: nombre
		sf::Event eventos;
		
		//x.pollEvent(Event.nombre): retorna TRUE si hay eventos pendientes
		while ( ventana.pollEvent( eventos ) )
		{
			
			switch ( eventos.type )
			{

			 case sf::Event::Closed:
				 //'Closed' event: cierra la ventana del juego
					ventana.close();
				break;
			//	-------------------------CONTROLES DEL JUEGO--------------------------\\
			 Event::KeyPressed: permite controlar las teclas
			 case sf::Event::KeyPressed:
				 //si presionamos ARRIBA
				 rotacion = ( eventos.key.code == sf::Keyboard::Up ) ? true : false;
				 //si presionamos DERECHA O IZQUIERDA
				 dx = ( eventos.key.code == sf::Keyboard::Right ) ? 1 : (eventos.key.code == sf::Keyboard::Left) ? -1: 0;
				break;
			}
			
		}
		
		//------------------------------------------LOGICA DEL JUEGO(UPDATE)----------------------------\\
		//caida mas rapido
		if ( eventos.key.code == sf::Keyboard::Down ) delay = 0.05;

		//-----------MOVIMIENTO EN LAS BARRAS---------------//
		for ( int i = 0; i < 4; i++ )
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		
		if ( !Check() )
		{
			for ( int i = 0; i < 4; i++ )
			{
				a[i] = b[i];
			}
		}
		//--------ROTACION--------//
		//si hay rotacion, recalcula la posicion del cubo
		if ( rotacion )
		{
			//coordenada del centro de la barra(El segundo cubo NO se mueve)
			Punto centro = a[1];
			
			//Rotar la barra a partir de su centro (a[1])
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - centro.y;
				int y = a[i].x - centro.x;
				//asingar cambios
				a[i].x = centro.x - x;
				a[i].y = centro.y + y;
			}
		}

		//---------------------------CAIDA DE LAS BARRAS------------------------\\
		 forzamos que caiga aumentando las coodenadas Y de los cubos
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}

			if ( !Check() )
			{
				for (int i = 0; i < 4; i++)
				{
					grid[ b[i].y ][ b[i].x ] = colorNum;
				}
				colorNum = 1 + rand() % 7;
				int n = rand() % 7;

				for (int i = 0; i < 4; i++)
				{
					a[i].x = figuras[n][i] % 2;
					a[i].y = figuras[n][i] / 2;
				}
			}
			//evita que siga callendo
			timer = 0;
		}

		//------------------------------CHEKEAR SI HAY LINEAS COMPLETAS-------------------//
		int l = FILAS - 1;

		for (int i = FILAS -1 ; i > 0; i--)
		{	
			int contador = 0;

			for ( int j = 0;  j < COLUMNAS;  j++)
			{
				if ( grid[i][j] ) contador++;
				grid[l][j] = grid[i][j];
			}
			
			if ( contador < COLUMNAS ) l--;
		}
		
		//detiene el cubo tras moverse una casilla
		dx = 0;
		//detiene la rotacion tras ejecutarla
		rotacion = false;

		//evita que siga en 0.05 al soltar la tecla
		delay = 0.4f;

		//-------------LIMPIA LA VENTANA CADA FRAME-------------------\\
		x.isClear(color de fondo): actualiza la ventana
		ventana.clear( sf::Color::White );
		ventana.draw( fondo );

		//--------------------DIBUJAR OBJETOS--------------------------\\
		x.draw(Sprite.name): dibuja el sprite en pantalla
		//empieza a dibujar Cubo a Cubo la barra
		for ( int i = 0; i < FILAS; i++ )
		{
			for ( int j = 0; j < COLUMNAS; j++ )
			{
				//
				if ( grid[i][j] == 0 ) continue;

				//seleccion de color de cubo por coodenadas
				barras.setTextureRect( sf::IntRect( grid[i][j] * 18, 0, 18, 18) );
				//coodenadas
				barras.setPosition( j * 18, i * 18 );
				//mostrar barra
				barras.move(28, 31); //offset
				ventana.draw( barras );
			}
		}
		for ( int i = 0; i < 4; i++ )
		{
			//actualiza el color de las barras
			barras.setTextureRect( sf::IntRect( colorNum * 18, 0, 18, 18) );
			//actualiza las coordenadas de la barra

			barras.setPosition( a[i].x * 18, a[i].y * 18 );

			//offset x,y
			barras.move( 28, 31 );
			//mostrar barra
			ventana.draw( barras );
		}

		//--------------MOSTRAR POR PANTALLA-------------------------\\
		x.display(): muestra los elementos al empezar el FRAME
		ventana.draw( marcos );
		ventana.display();
	}

	return 0;
}