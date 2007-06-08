import java.net.ServerSocket;
import java.net.Socket;
import java.io.FileInputStream;
import java.io.OutputStream;

public class test{
    public static void main(String[]args)throws Exception{
	System.out.println("bla");
	FileInputStream fis=new FileInputStream("test.108.ppm");
	Server server=new Server();
	server.start();
//	int size=fis.available();
	int size=500000;
	byte[]buffer=new byte[size];
	for(int a = 0;a<size;a++){
	    buffer[a]='a';
	}
	System.out.println("size:"+buffer.length);

//	fis.read(buffer);
	Socket client=new Socket("localhost", 10000);
	OutputStream os=client.getOutputStream();
	os.write(buffer);
	System.out.println("Daten gesendet");
	
	
	
    }
}


class Server extends Thread{
    ServerSocket socket;
    public Server()throws Exception{
	socket=new ServerSocket(10000);
	
    }
    
    public void run(){
	while(true){
	    try{
		Socket client=socket.accept();
//		Thread.sleep(1000);
		int counter=0;
		int recvSize=client.getInputStream().available();
		while(recvSize>0){
		    recvSize=client.getInputStream().available();
		    byte[]buffer=new byte[recvSize];
		    int bytes=client.getInputStream().read(buffer);
		    counter+=bytes;
//		    Thread.sleep(500);
		    System.out.println("recvsize:"+recvSize+" received:"+bytes+" BYTES:"+buffer.length);
		}
		System.out.println("recvsizeAll:"+counter);

	    }catch(Exception ex){
		ex.printStackTrace();
	    }
	    }
	}
    
}