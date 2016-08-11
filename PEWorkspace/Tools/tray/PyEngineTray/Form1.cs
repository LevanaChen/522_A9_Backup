using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System;
using System.IO;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void quitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void pyEngineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Create process
            System.Diagnostics.Process pProcess = new System.Diagnostics.Process();

            //strCommand is path and file name of command to run
            ;
            pProcess.StartInfo.FileName = Directory.GetCurrentDirectory() + "/../rundevenv.bat";

            //strCommandParameters are parameters to pass to program
            pProcess.StartInfo.Arguments = "";

            pProcess.StartInfo.UseShellExecute = true;

            //Set output of program to be written to process output stream
            pProcess.StartInfo.RedirectStandardOutput = false;

            //Optional
            pProcess.StartInfo.WorkingDirectory = Directory.GetCurrentDirectory() + "/..";

            //Start the process
            pProcess.Start();

            //Get program output
            //string strOutput = pProcess.StandardOutput.ReadToEnd();

            //Wait for process to finish
            //pProcess.WaitForExit();
        }

        private void mayaToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Create process
            System.Diagnostics.Process pProcess = new System.Diagnostics.Process();

            //strCommand is path and file name of command to run
            ;
            pProcess.StartInfo.FileName = Directory.GetCurrentDirectory() + "/../runmaya.bat";

            //strCommandParameters are parameters to pass to program
            pProcess.StartInfo.Arguments = "";

            pProcess.StartInfo.UseShellExecute = true;

            //Set output of program to be written to process output stream
            pProcess.StartInfo.RedirectStandardOutput = false;

            //Optional
            pProcess.StartInfo.WorkingDirectory = Directory.GetCurrentDirectory() + "/..";

            //Start the process
            pProcess.Start();

            //Get program output
            //string strOutput = pProcess.StandardOutput.ReadToEnd();

            //Wait for process to finish
            //pProcess.WaitForExit();
        }

        private void pyClientToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Create process
            System.Diagnostics.Process pProcess = new System.Diagnostics.Process();

            //strCommand is path and file name of command to run
            ;
            pProcess.StartInfo.FileName = Directory.GetCurrentDirectory() + "/../pyclient.bat";

            //strCommandParameters are parameters to pass to program
            pProcess.StartInfo.Arguments = "";

            pProcess.StartInfo.UseShellExecute = true;

            //Set output of program to be written to process output stream
            pProcess.StartInfo.RedirectStandardOutput = false;

            //Optional
            pProcess.StartInfo.WorkingDirectory = Directory.GetCurrentDirectory() + "/..";

            //Start the process
            pProcess.Start();

            //Get program output
            //string strOutput = pProcess.StandardOutput.ReadToEnd();

            //Wait for process to finish
           // pProcess.WaitForExit();
        }

        private void deployEngineToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Create process
            System.Diagnostics.Process pProcess = new System.Diagnostics.Process();

            //strCommand is path and file name of command to run
            ;
            pProcess.StartInfo.FileName = Directory.GetCurrentDirectory() + "/../Deployment/DeployPyengine2.bat";

            //strCommandParameters are parameters to pass to program
            pProcess.StartInfo.Arguments = "";

            pProcess.StartInfo.UseShellExecute = true;

            //Set output of program to be written to process output stream
            pProcess.StartInfo.RedirectStandardOutput = false;

            //Optional
            pProcess.StartInfo.WorkingDirectory = Directory.GetCurrentDirectory() + "/..";

            //Start the process
            pProcess.Start();

            //Get program output
            //string strOutput = pProcess.StandardOutput.ReadToEnd();

            //Wait for process to finish
            // pProcess.WaitForExit();
        }

        private void notifyIcon1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            this.notifyIcon1.Visible = true;
            this.notifyIcon1.ShowBalloonTip(30000);
        }

        private void notifyIcon1_MouseDoubleClick_1(object sender, MouseEventArgs e)
        {

        }
    }
}
