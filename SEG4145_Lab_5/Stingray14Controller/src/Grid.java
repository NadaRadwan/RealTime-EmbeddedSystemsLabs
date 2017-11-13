import java.awt.Container;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusListener;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.awt.event.FocusEvent;
import java.awt.Dimension;
import java.awt.Color;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.BorderFactory;
import javax.swing.border.Border;


public class Grid extends JFrame implements ActionListener, FocusListener{
	
	  private String[] buttonLabels = {
            "Move forward.[0-20 cm]",
            "Move backward.[0-20 cm]",
            "Rotate cw.[0-359 deg]",
            "Rotate ccw.[0-359 deg]",
            "Nearest object.",
            "Read temperature.",
            "Quit"
          };
	
	  private Client client;
	  JTextArea input;
	  JTextArea output;
	  
	  public void actionPerformed(ActionEvent e) {
		  String badParamMessage = "This command requires a parameter.\nProvide one here and try again.\n";
		  if(input.getText().equals(badParamMessage) ||
				  input.getText().equals("Invalid parameter [0-20 cm]") ||
				  input.getText().equals("Invalid parameter [0-359 deg]")) {
			  return;
		  }
		  JButton button = (JButton) e.getSource();
		  String sCommand = button.getActionCommand();
		  int iCommand = -1;
		  for(int i = 0; i < buttonLabels.length; i++) {
			  if(sCommand == buttonLabels[i]) {
				  iCommand = i + 1;
				  break;
			  }
		  }
		  if(input.getText().equals("") && iCommand < 5) {
			  input.setText(badParamMessage);
		  }
		  else if(iCommand == 7) {
			  this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			  System.exit(0);
		  }
		  else if(iCommand < 3 && Integer.parseInt(input.getText()) > 20) {
			  input.setText("Invalid parameter [0-20 cm]");
		  }
		  else if(iCommand < 5 && Integer.parseInt(input.getText()) > 359) {
			  input.setText("Invalid parameter [0-359 deg]");
		  }
		  else{
			  try {
				  // String returnValue = this.client.sendToServer(sCommand, input.getText());
				  Date d = new Date();
				  SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd:HH-mm");
				  String sd = sdf.format(d);
				  if(iCommand < 5) {
					  output.append(sd + " : " + sCommand.split("\\.")[0] + " " + input.getText() + "\n");
				  }
				  else {
				  //  output.append(sd + " : " + sCommand.split("\\.")[0] + " Val: " + returnValue + "\n");
				  }
				  input.setText("");
			  }
			  catch(Exception exc) {
				  System.out.println("Something went wrong when sending to server.");
				  System.out.println(exc);
			  }
		  }
	  }
	  
      public void focusGained(FocusEvent e) {
    	  input.setText("");
      }
      
      public void focusLost(FocusEvent e) {
    	  // do nothing (part of interface)
      }
		
	  public Grid(/*Client c*/) {
		// this.client = c;
	    Container pane = getContentPane();
	    pane.setLayout(new GridLayout(3, 3));
	    JButton[] buttons = new JButton[7];
	    for(int i=0; i<7; i++) {
	    	buttons[i] = new JButton(buttonLabels[i]);
	    	buttons[i].setFont(new Font("Courier", Font.PLAIN, 16));
	    	buttons[i].setPreferredSize(new Dimension(300, 300));
	    	buttons[i].addActionListener(this);
		    pane.add(buttons[i]);
	    }
	    
	    output = new JTextArea();
	    output.setFont(output.getFont().deriveFont(16f));
	    output.setEditable(false);
	    output.setBorder(BorderFactory.createLineBorder(Color.BLUE, 2));
	    pane.add(output);
	    
	    input = new JTextArea();
	    input.setText("");
	    input.setFont(input.getFont().deriveFont(16f));
	    input.setEditable(true);
	    input.setBorder(BorderFactory.createLineBorder(Color.BLUE, 2));
	    input.addFocusListener(this);
	    pane.add(input);

	  }

	}