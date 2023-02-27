package main

import (
	"Network-go/network/bcast"
	"Network-go/network/localip"
	"Network-go/network/peers"
	"flag"
	"fmt"
	"os"
	"os/exec"
	"time"
)

const COUNT_INTERVAL = 1

func main() {

	isBackup := true

	var isFirst int
	flag.IntVar(&isFirst, "isFirst", 1, "init first node different from rest")

	var id string
	flag.StringVar(&id, "id", "", "id of this peer")
	flag.Parse()

	if(isFirst != 0){
		isBackup = false
		make_backup()		
	}

	// ... or alternatively, we can use the local IP address.
	// (But since we can run multiple programs on the same PC, we also append the
	//  process ID)
	if id == "" {
		localIP, err := localip.LocalIP()
		if err != nil {
			fmt.Println(err)
			localIP = "DISCONNECTED"
		}
		id = fmt.Sprintf("peer-%s-%d", localIP, os.Getpid())
	}

	// We make a channel for receiving updates on the id's of the peers that are
	//  alive on the network
	peerUpdateCh := make(chan peers.PeerUpdate)
	// We can disable/enable the transmitter after it has been started.
	// This could be used to signal that we are somehow "unavailable".
	peerTxEnable := make(chan bool)
	go peers.Transmitter(15647, id, peerTxEnable)
	go peers.Receiver(15647, peerUpdateCh)


	countTx := make(chan int)
	countRx := make(chan int)

	go bcast.Transmitter(16569, countTx)
	go bcast.Receiver(16569, countRx)

	count_timer := time.NewTimer(COUNT_INTERVAL*time.Second)
	count := 0

	for {
		select {
		case p := <-peerUpdateCh:
			if (len(p.Lost) > 0){
				isBackup = false
				make_backup()
			}
		case new_count := <-countRx:
			if isBackup {
				count = new_count
			}
		case <-count_timer.C:
			count_timer.Reset(COUNT_INTERVAL*time.Second)
			if !isBackup {
				count++
				countTx <- count
				println(count)
			}

		}
	}
}

func make_backup(){
	exec.Command("gnome-terminal","--","go","run","main.go","-isFirst=0").Run()
}