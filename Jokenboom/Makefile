all:
# mkdir -> Criar diretório bin
	mkdir -p ./bin 
	gcc -Wall client.c common_module.c -o ./bin/client
	gcc -Wall server.c common_module.c -o ./bin/server

clean:
	rm -f ./bin/client ./bin/server
