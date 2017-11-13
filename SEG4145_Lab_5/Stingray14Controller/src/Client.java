import java.net.Socket;
import java.io.*;
import java.net.*;

public class Client {

    private int port;
    private String host;
    private Socket socket;

    private BufferedWriter bw; //output
	
	//constructor
	public Client(){
		port = 80;
		host = "192.168.1.188";
		
		try{
			this.socket = new Socket(host, port);
			System.out.println("connected to server");
			OutputStream outStream = this.socket.getOutputStream();
	        bw = new BufferedWriter(new OutputStreamWriter(outStream));
		} catch (UnknownHostException e){
			System.out.println("Connection to server on port "+port+" failed due to invalid host.");
		    System.exit(1); //exit the client
		} catch (NumberFormatException e) {
			System.out.println(e);
		    System.exit(1);
		} catch (IOException e) {
			System.out.println(e);
			System.exit(1);
		}
	}
	
    public void sendToServer(String command, String param) throws IOException {
        bw.write(command + " " + param + "\n");
        bw.flush();
    }
}

		
		
	
      
   

        

