# Introduzione #
In questa sezione si analizzano, in modo strutturato, gli scenari di casi d'uso per operazioni più complesse.

---

# UC-Aggiungi Interfaccia #

**Attore:** utente finale.

**Contesto:** applicazione principale, aggiunta di una nuova interfaccia di rete ad una VM.

**Pre-condizioni:** sistema avviato, esiste un lab con almeno una VM che è la VM selezionata.

**Post-condizioni:** l'interfaccia è stata creata ed associata alla VM.

**Scenario Principale di Successo:** l'utente, dopo aver selezionato la VM a cui vuole aggiungere l'interfaccia, clicca col tasto destro sulla VM e sceglie la voce "Add New Interface". L'utente specifica quindi il tipo di interfaccia "Network".

**Varianti:**
  1. l'utente seleziona la VM. Dal menu "Edit", sceglie la voce "Add New Interface" e specifica il tipo di interfaccia "Network";
  1. dal menu "Edit" l'utente sceglie la voce "Add New Interface" e specifica il tipo di interfaccia "Network" e la VM a cui associarla;
  1. dalla barra dei componenti l'utente trascina l'oggetto "Network Interface" sulla VM di destinazione.



# UC-Connetti due VMs #

**Attore:** utente finale.

**Contesto:** applicazione principale, aggiunta di una connessione di rete tra due VMs.

**Pre-condizioni:** sistema avviato, esiste un lab con almeno due VMs.

**Post-condizioni:** la connessione è stata creata ed associata alle VMs.

**Scenario Principale di Successo:** l'utente seleziona due macchine virtuali VM1 e VM2, clicca con il tasto destro e sceglie la voce "Add New Connection". L'utente specifica il tipo di connessione "Network".

**Varianti:**
  1. l'utente, dopo aver selezionato le due VMs, clicca sul menu "Edit" e sceglie la voce "Add New Connection". L'utente specifica il tipo di connessione "Network" ed inserisce i parametri di configurazione di base: nome, identificativo, indirizzo;
  1. l'utente clicca sul menu "Edit" e sceglie la voce "Add New Connection". L'utente specifica il tipo di connessione "Network", specifica le due VMs coinvolte ed inserisce i parametri di configurazione di base: nome, identificativo, indirizzo.



# UC-Crea un gruppo di VMs #

**Attore:** utente finale.

**Contesto:** applicazione principale, creazione di un gruppo di VMs.

**Pre-condizioni:** sistema avviato, esiste un lab con almeno una VM.

**Post-condizioni:** il gruppo è stato creato e le VMs vi sono state associate.

**Scenario Principale di Successo:** l'utente seleziona una o più macchine virtuali, clicca con il tasto destro e sceglie la voce "Group". L'utente specifica il tipo di gruppo tra "AS","Area" ed inserisce alcune informazioni di base: nome, identificativo.

**Varianti:**
  1. l'utente, dopo aver selezionato le VMs, clicca sul menu "Edit" e sceglie la voce "Group". L'utente specifica il tipo di gruppo ed inserisce alcune informazioni di base: nome, identificativo;
  1. l'utente clicca sul menu "Edit" e sceglie la voce "Group". L'utente specifica il tipo di gruppo, le VM coinvolte ed inserisce alcune informazioni di base: nome, identificativo.