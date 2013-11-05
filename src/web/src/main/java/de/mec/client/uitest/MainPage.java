/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.uitest;

import com.google.gwt.core.client.GWT;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Widget;

/**
 *
 * @author jan.hoelscher
 */
public class MainPage extends Composite {
    
    private static MainPageUiBinder uiBinder = GWT.create(MainPageUiBinder.class);
    
    interface MainPageUiBinder extends UiBinder<Widget, MainPage> {
    }
    
    public MainPage() {
        initWidget(uiBinder.createAndBindUi(this));
    }
}
