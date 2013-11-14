/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client.editor;

import com.sencha.gxt.data.shared.Converter;
import com.sencha.gxt.widget.core.client.form.Field;
import com.sencha.gxt.widget.core.client.form.TextField;
import de.mec.client.PropertyItemEditor;

/**
 *
 * @author jan.hoelscher
 */
public class HiddenItemEditor extends PropertyItemEditor {

    public HiddenItemEditor() {
        TextField field = new TextField();
        field.setVisible(false);
        setComponent(field);
    }
}
