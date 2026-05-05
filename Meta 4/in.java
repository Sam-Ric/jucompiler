class Main {
    public static int x; // Variável global

    public static void main(String[] args) {
        x = 100; // Escreve na global
        
        System.out.print("Global x inicial: ");
        System.out.print(x);
        System.out.print("\n");

        if (x > 0) {
            // 1. Variável local escondida dentro de um bloco
            int escondida;
            escondida = 42; 
            System.out.print("Variavel escondida: ");
            System.out.print(escondida);
            System.out.print("\n");

            // 2. Variável local a fazer SHADOWING dentro de um bloco
            int x; 
            x = 999; // Deve escrever na local, não na global!
            System.out.print("Local x (shadowing): ");
            System.out.print(x);
            System.out.print("\n");
        }

        // Se o compilador estiver certo, o x global continua a ser 100
        System.out.print("Global x final: ");
        System.out.print(x);
        System.out.print("\n");
    }
}