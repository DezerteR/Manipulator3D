// cos jakby issues, bliższe todo list, do issues nie chce mi sie patrzec
- bullet
- lua
- joint interpolated movement
- teaching

- dodać czas do komend
- uogólnić interpolatory na zestawy {glm::vec4}, {vec4, quat}, {vector<double>}, najlepiej templatkowo, a odpowiednim
	kontenerom zapewnić odpowiednie operatory


==== joint interpolated movement ====
	konstruktor ma dwie opcje:
	- dostajemy zestaw wspóżednych zlaczowych i jestemy happy
	- dostajemy zestaw punktów, zlaczowe musimy sobie wyliczyc
	dodatkowo dostajemy zestaw modyfiaktorów predkosci
	interpolator sobie wybieramy
	
==== teaching =====
	co klatke zbieramy pozycje robota: vec4, quat i joint, w sumie to wystarczy joint, reszta jest wyliczalna
	przy pushu należy pamitać o tym żeby wygadzić punkty, albo można wykorzystać solver z lerpem do ledzenia pozycji
	punkt niech bdzie poruszany przez pointController z edytora, czemu wiec nie przeniesc by teachingu do 
	interpolatorów?
	
==== sledzenie trasy ====
	w kazdym frame wyliczamy o ile trzeba sie przemiecić, nastpnie zdejmujemy z interpolatora punkty aż przemieszczenie nie bedzie wieksze niz zakadane, potem można zinterpolować pomidzy punktami, ale po chuj?
	
==== ogólne ====
	graf ma defaultowy tryb LastX, trzeba mu zrobić opcje exportu danych dla matlaba
	dać możliwoć logowania wiekszej iloci danych
	
==== punkty ====
	mozliwosc zapisania pozycji robota {vec4, quat, vector<float>} do mapy/vectora, z menu, komendy savePoint()
	punkty te maja generyczne nazwy Px, 
	vector bedzie lepszy
	zrobic tak by dalo sie punkty wywietlic na 3D, jakos korzystac w interpolatorach, np goTo(point("P123")+glm::vec4(0,0,13));
	
==== zrobić w końcu klase Label bedaca const charem[] o staym rozmiarze, np. 30 znaków, 
	struct InfoLabel {
		char word[31];
		char size;
		InfoLabel(const char* _word){
			std::copy(_word, strlen(_word), word);
		}
		InfoLabel(const string _word){
			std::copy(_word.begin(), _word.end(), word);
		}
		InfoLabel(const InfoLabel&) = delete;
		InfoLabel(const InfoLabel&&) = delete;
		InfoLabel& operator = (const InfoLabel&) = delete;  
	};
	powinno sie ladnie alignować.

==== 
	

