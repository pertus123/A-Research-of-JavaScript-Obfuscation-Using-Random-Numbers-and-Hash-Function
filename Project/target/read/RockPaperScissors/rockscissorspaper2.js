function Player(p) {
    if (!p) {
        p = 0;
    }
    this.point = p;
}
//sadsadsad
Player.prototype.toString = function() {
    switch(this.point) {
        case 0:
            return "가위";
        case 1:
            return "바위";
        case 2:
            return "보";
    }
};
//aaaa
function playRockScissorsPaper(evt) {

    var source = getEventSource(evt);
    var player = new Player();
    var computer = new Player(Math.floor(Math.random() * 3));
    var resultElement = document.getElementById('result');
 
    switch(source.id) {
        case 'btnScissors':
            player.point = 0;
            break;
        case 'btnRock':
            player.point = 1;
            break;
        case 'btnPaper':
            player.point = 2;
            break;
        default:
            return; //zzzzzzzzz
    }

    if(player.point == computer.point) {
        resultElement.innerHTML = '플레이어: ' + player + ' vs 컴퓨터: ' + computer + ' => 비겼습니다!!!';
    } else {
        if ((player.point + 1) % 3 == computer.point) {


            resultElement.innerHTML = '플레이어: ' + player + ' vs 컴퓨터: ' + computer + ' => 졌습니다!!!';


        } else {
            resultElement.innerHTML = '플레이어: ' + player + ' vs 컴퓨터: ' + computer + ' => 이겼습니다!!!';
        }
    };
}
function clearResult(){
        var resultElement = document.getElementById("result");
        resultElement.innerHTML = "준비";
}
function init() {
        var btnRockElement = document.getElementById("btnRock");
        var btnScissorsElement = document.getElementById("btnScissors");
        var btnPaperElement = document.getElementById("btnPaper");
        var btnResetElement = document.getElementById("btnReset");
        addListener2(btnRockElement, 'click', playRockScissorsPaper);
        addListener2(btnScissorsElement, 'click', playRockScissorsPaper);
        addListener2(btnPaperElement, 'click', playRockScissorsPaper);
        addListener2(btnResetElement, 'click', clearResult);
}



function addListener2(el, ev, handler) {

    if (el.addEventListener) {
        el.addEventListener(ev, handler, false);
    } else {
        el.attachEvent('on' + ev, handler);
    }
}


function getEventSource(evt) {
    if (evt.target)
        return evt.target;
    else
        return window.event.srcElement;
}
addListener2(window, 'load', init);
