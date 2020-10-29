var counter = 0;
var phrase = [
    '“All our dreams can come true, if we have the courage to pursue them.” – Walt Disney',
    '“The secret of getting ahead is getting started.” – Mark Twain',
    '“Don’t limit yourself. Many people limit themselves to what they think they can do. You can go as far as your mind lets you. What you believe, remember, you can achieve.” – Mary Kay Ash',
    '“The best time to plant a tree was 20 years ago. The second best time is now.” – Chinese Proverb',
    '“It’s hard to beat a person who never gives up.” – Babe Ruth',
    '“If people are doubting how far you can go, go so far that you can’t hear them anymore.” – Michele Ruiz',
    '“Fairy tales are more than true: not because they tell us that dragons exist, but because they tell us that dragons can be beaten.”― Neil Gaiman',
    '“Everything you can imagine is real.”― Pablo Picasso',
    '“Smart people learn from everything and everyone, average people from their experiences, stupid people already have all the answers.” – Socrates',
    '“Do what you feel in your heart to be right – for you’ll be criticized anyway.”― Eleanor Roosevelt'
    ];

function game()
{
    counter++;
    switch(counter){
        case 1:
            document.querySelector('#titulo').innerHTML = "¿Quién tiene más aguante?";
            document.querySelector('#first').src = "https://s.libertaddigital.com/2019/10/11/1920/1080/fit/cristiano-ronaldo-portugal.jpg";
            document.querySelector('#second').src = "https://www.itl.cat/pngfile/big/243-2430833_messi-argentina-wallpaper-for-desktop-with-image-resolution.jpg";
            document.querySelector('#bot1').innerHTML = "Cristiano Ronaldo";
            document.querySelector('#bot2').innerHTML = "Lionel Messi";
            break;
        case 2:
            document.querySelector('#titulo').innerHTML = "¿Mejor mascota?";
            document.querySelector('#first').src = "https://www.ecestaticos.com/imagestatic/clipping/9da/18c/9da18cdc3b033ca0375e7b9c1394c0d0/has-conseguido-que-llore-perdio-a-su-perro-pidio-ayuda-en-las-redes-y-esto-es-lo-que-paso.jpg?mtime=1579565836";
            document.querySelector('#second').src = "https://i.pinimg.com/originals/c7/39/87/c73987ec05ce829f4ca959d1c1f576c0.jpg";
            document.querySelector('#bot1').innerHTML = "Perros";
            document.querySelector('#bot2').innerHTML = "Gatos";
            break;
        case 3:
            document.querySelector('#titulo').innerHTML = "¿Lenguaje más piola?";
            document.querySelector('#first').src = "https://afortic.org/images/detailed/3/oracle-java.jpg";
            document.querySelector('#second').src = "https://engineering.fb.com/wp-content/uploads/2015/06/1522635669452_11.jpg";
            document.querySelector('#bot1').innerHTML = "Java";
            document.querySelector('#bot2').innerHTML = "C/C++";
            break;
        case 4:
            document.querySelector('#titulo').innerHTML = "¿Quién gana un 1 vs 1?";
            document.querySelector('#first').src = "https://i.pinimg.com/originals/4f/1d/b4/4f1db43d0123fcd8905fade8459480db.jpg";
            document.querySelector('#second').src = "https://www.syfy.com/sites/syfy/files/styles/1200x680/public/wire/legacy/samuel-l-jackson-mace-windu.jpg";
            document.querySelector('#bot1').innerHTML = "Master Yoda";
            document.querySelector('#bot2').innerHTML = "Master Windu";
            break;
        case 5:
            document.querySelector('#botones').style.visibility = 'hidden';
            document.querySelector('#cartel').style.visibility = 'visible';
    }
}

function quotes()
{
    let num = Math.floor((Math.random() * 10));
    document.querySelector('#frase').innerHTML = phrase[num];
}