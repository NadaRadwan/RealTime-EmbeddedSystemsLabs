package lab4_new;

import java.net.Socket;
import java.util.Scanner;
import java.io.*;
import java.net.*;

public class Client {

	    private Scanner scanner;
	    private int port;
	    private String host;
	    private Socket socket;

	    private BufferedWriter bw; //output
	
	//constructor
	public Client(){
		scanner = new Scanner(System.in);
		port = 80;
		host = "192.168.1.188";
		
		try{
			this.socket = new Socket(host, port);
			System.out.println("connected to server");
			OutputStream outStream = this.socket.getOutputStream();
	        bw = new BufferedWriter(new OutputStreamWriter(outStream));
		}catch (UnknownHostException e){
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
	
	public void acceptInput(){
		
		while(true){
            displayMenuOptions();   
            String input = scanner.nextLine();
            
            int value;
            try{
            	 value = validateInput(input);
            }catch(NumberFormatException e){
            	 System.out.println("This is an invalid command");
                 continue;
            }
            try{
	            switch (value){
	                case 1:
	                    moveForward();
	                    break;
	                case 2:
	                    moveBackward();
	                    break;
	                case 3:
	                    rotateClockwise();
	                    break;
	                case 4:
	                    rotateCounterClockwise();
	                    break;
	                case 5:
	                    readDistance();
	                    break;
	                case 6:
	                    readTemperature();
	                    break;
	                case 7:
	                    quit();
	                    break;
	            }
            }catch(IOException e){
            	System.out.println("Unable to send to server");
                continue; //continue to loop
            }
        }
	}
	
	/*
	 * converts string to integer and throws exception if invalid input
	 */
	private int validateInput(String input) throws NumberFormatException{
         return Integer.parseInt(input.trim());
	}
	
	private void displayMenuOptions() {
		System.out.println("Enter the correct number to select an operation:");
		System.out.println("1 – Move the robot forward.");
		System.out.println("2 – Move the robot backward.");
		System.out.println("3 – Rotate the robot clockwise.");
		System.out.println("4 – Rotate the robot counter clockwise.");
		System.out.println("5 – Read the distance to the nearest object.");
		System.out.println("6 – Read temperature values.");
		System.out.println("7 – Quit.");		
	}
	
	 /*
     * if invalid value entered, send 0cm or 0 degrees
     */
    private int getFollowUpInput(){
    	String input = scanner.nextLine();
    	
    	int value = 0; //set it to 0 so that no movement happens
        try{
        	 value = validateInput(input);
        }catch(NumberFormatException e){
        	 System.out.println("This is an invalid input");
        }  
        return value;
    }
	
	 public void moveForward() throws IOException{
		 System.out.println("How many cm to move forward (0-20 cm)?");
		 boolean valid = false;
		 int i=-1;
		 while (!valid){
		        i = getFollowUpInput();
		        if(i>=0 && i<=20){
		        	valid=true;
		        }else{
		        	System.out.println("Please enter a value in range (0-20cm)");
		        }
		 }
		 sendToServer("1 "+i);
	    }
	    public void moveBackward() throws IOException{
	        System.out.println("How many cm to move backwards (0-20 cm)?");
	        boolean valid = false;
	        int i=-1;
			 while (!valid){
			        i = getFollowUpInput();
			        if(i>=0 && i<=20){
			        	valid=true;
			        }else{
			        	System.out.println("Please enter a value in range (0-20cm)");
			        }
			 }
	        sendToServer("2 "+i);
	    }
	    public void rotateClockwise() throws IOException {
	    	System.out.println("How many degrees to rotate clockwise (0-359 degrees)?");
	    	 boolean valid = false;
		        int i=-1;
				 while (!valid){
				        i = getFollowUpInput();
				        if(i>=0 && i<=359){
				        	valid=true;
				        }else{
				        	System.out.println("Please enter a value in range (0-359 degrees)");
				        }
				 }
	        sendToServer("3 "+i);
	    }
	    public void rotateCounterClockwise() throws IOException {
	    	System.out.println("How many degreesto rotate counterclockwise (0-359) ?");
	    	boolean valid = false;
	        int i=-1;
			 while (!valid){
			        i = getFollowUpInput();
			        if(i>=0 && i<=359){
			        	valid=true;
			        }else{
			        	System.out.println("Please enter a value in range (0-359 degrees)");
			        }
			 }
	        sendToServer("4 "+i);
	    }
	    public void readDistance() throws IOException {
	    	sendToServer("5");
	    }
	    public void readTemperature() throws IOException {
	    	sendToServer("6");
	    }
	    public void quit() throws IOException {
	    	sendToServer("7");
	    	System.out.println("terminating client");
	        bw.close(); //close bufferedWriter
	        socket.close(); //close socket
	        System.exit(1);
	    }
	    private void sendToServer(String value) throws IOException {
	        bw.write(value+"\n");
	        bw.flush();
	    }
}

		
		
	
      
   

        

