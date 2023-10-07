#include <stdio.h>

int main() {
    printf("ml' nob:\n");
    int input, inp;
    char flo;
    inp = scanf(" %d %c ", &input, &flo);
    if(inp == 1){
        switch (input) {
            case 0:
                printf("Qapla'\nnoH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n");
                break;
            case 1:
                printf("Qapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n");
                break;
            case 2:
                printf("Qapla'\nQu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n");
                break;
            case 3:
                printf("Qapla'\nbISeH'eghlaH'be'chugh latlh Dara'laH'be'.\n");
                break;
            case 4:
                printf("Qapla'\nqaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n");
                break;
            case 5:
                printf("Qapla'\nSuvlu'taHvIS yapbe' HoS neH.\n");
                break;
            case 6:
                printf("Qapla'\nHa'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n");
                break;
            case 7:
                printf("Qapla'\nHeghlu'meH QaQ jajvam.\n");
                break;
            case 8:
                printf("Qapla'\nleghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
                break;
            default:
                printf("Qih mi' %d\n", input);
                break;
        }
    }
    else if(inp == 2){
        printf("bIjatlh 'e' yImev\n");
    }
    else {
        printf("Neh mi'\n");
        return 1;
    }
    return 0;
}
